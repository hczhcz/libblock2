#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

void NodeCall::renderFrameAlloc(
    Instance &instance, size_t position,
    OutputContext &oc
) const {
    switch (mode) {
        case FrameMode::static_global:
            oc.endl();
            oc.os << "static "
                  << instance.strCalleeType(position) << " "
                  << instance.strCalleeName(position) << ";";
            oc.endl();
            oc.os << "inner = (struct frame *) &"
                  << instance.strCalleeName(position) << ";";
            break;

        case FrameMode::static_local:
            oc.endl();
            oc.os << instance.strCalleeType(position) << " "
                  << instance.strCalleeName(position) << ";";
            oc.endl();
            oc.os << "inner = (struct frame *) &"
                  << instance.strCalleeName(position) << ";";
            break;

        case FrameMode::dynamic_stack:
            oc.endl();
            oc.os << "inner = alloca(sizeof("
                  << instance.strCalleeType(position)
                  << "));";
            break;

        case FrameMode::dynamic_gc:
            oc.endl();
            oc.os << "inner = GC_malloc(sizeof("
                  << instance.strCalleeType(position)
                  << "));";
            break;

        case FrameMode::dynamic_free:
            oc.endl();
            oc.os << "inner = malloc(sizeof("
                  << instance.strCalleeType(position)
                  << "));";
            break;
    }
}

void NodeCall::renderLoadCallee(OutputContext &oc) const {
    oc.endl();
    oc.os << "inner->outer = callee;";
    oc.endl();
    oc.os << "callee = inner;";
}

void NodeCall::renderBeforeCall(
    OutputContext &oc,
    const std::string &label
) const {
    oc.endl();
    oc.os << "self->func = &&" << label << ";";

    oc.endl();
    oc.os << "callee->caller = self;";
    oc.endl();
    oc.os << "self = callee;";
}

void NodeCall::renderCall(
    OutputContext &oc,
    const std::string &func
) const {
    // notice: reset self->func
    oc.endl();
    oc.os << "self->func = &&"
          << func << ";";
    oc.endl();
    oc.os << "goto *self->func;";
}

void NodeCall::renderAfterCall(
    OutputContext &oc,
    const std::string &label
) const {
    oc.endl();
    oc.os << label << ":";

    oc.endl();
    oc.os << "callee = self;";
    oc.endl();
    oc.os << "self = callee->caller;";
}

void NodeCall::renderUnloadCallee(OutputContext &oc) const {
    oc.endl();
    oc.os << "inner = callee;";
    oc.endl();
    oc.os << "callee = inner->outer;";
}

void NodeCall::renderFrameFree(
    Instance &instance, size_t position,
    OutputContext &oc
) const {
    switch (mode) {
        case FrameMode::static_global:
        case FrameMode::static_local:
            break;

        case FrameMode::dynamic_stack:
            oc.endl();
            oc.os << "alloca(-sizeof("
                  << instance.strCalleeType(position)
                  << "));";
            break;

        case FrameMode::dynamic_gc:
        case FrameMode::dynamic_free:
            break;
    }
}

// class forkTry: a special solution

#include <unistd.h>

bool forkTry(std::function<void ()> &&func) {
    int pipefd[2];
    bool result;

    pipe(pipefd);

    if (fork()) {
        // parent process

        read(pipefd[0], &result, sizeof(result));

        return result;
    } else {
        // child process

        try {
            func();

            result = true;
        } catch (...) {
            result = false;
        }

        write(pipefd[1], &result, sizeof(result));
        exit(0);
    }
}

// class forkTry end

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Block &)> &&init,
    std::function<void (Instance &, size_t)> &&before,
    std::function<void (Instance &, size_t)> &&after
) {
    // special symbols:
    //     lookup: self, input, result, parent
    //     control flow: frame (func, outer, caller)

    // get callee

    Type &callee_type {
        source_p->buildOut(
            instance,
            output,
            [](Type &type) -> std::string {
                return type.strReint("tmp");
            }
        )
    };

    if (
        TypeClosure *closure_p {
            dynamic_cast<TypeClosure *>(&callee_type)
        }
    ) {
        size_t position {instance.addPosition()};

        // callee is closure
        // create a frame and call the function

        Type &parent {closure_p->parent};

        auto do_build = [&](Block &block) {
            block.build(
                output,
                [&](Instance &callee) {
                    // render (alloc the call frame)

                    output.content(
                        instance,
                        [&, position](OutputContext &oc) {
                            oc.endl();
                            oc.os << "/* call */";
                            oc.enter();

                            renderFrameAlloc(instance, position, oc);
                        }
                    );

                    // init

                    init(block);

                    if (block.getOption(BlockOption::parent)) {
                        // parent

                        callee.insert("parent", parent);

                        // render (parent)

                        output.content(
                            instance,
                            [&, position](OutputContext &oc) {
                                oc.endl();
                                oc.os << instance.strInner(position) << "->parent = "
                                      << parent.strReint("tmp") << ";";
                            }
                        );
                    }

                    // input

                    before(callee, position);

                    // render (load the callee)

                    output.content(
                        instance,
                        [&](OutputContext &oc) {
                            renderLoadCallee(oc);
                        }
                    );

                    // in args

                    for (size_t i = 0; i < args.size(); ++i) {
                        block.inArg(
                            instance, callee,
                            i, args[i],
                            output,
                            [&, position](Type &) {
                                return instance.strCallee(position);
                            }
                        );
                    }

                    // render (before call)

                    output.content(
                        instance,
                        [&, position](OutputContext &oc) {
                            renderBeforeCall(oc, instance.strLabel(position));
                        }
                    );
                },
                [&](Instance &callee) {
                    instance.addCallee(position, callee);

                    // render (after call)

                    output.content(
                        instance,
                        [&, position](OutputContext &oc) {
                            renderCall(oc, callee.strFunc());

                            renderAfterCall(oc, instance.strLabel(position));
                        }
                    );

                    // out args

                    for (size_t i = 0; i < args.size(); ++i) {
                        block.outArg(
                            instance, callee,
                            i, args[i],
                            output,
                            [&, position](Type &) {
                                return instance.strCallee(position);
                            }
                        );
                    }

                    // render (unload the callee)

                    output.content(
                        instance,
                        [&](OutputContext &oc) {
                            renderUnloadCallee(oc);
                        }
                    );

                    // result

                    after(callee, position);

                    // render (free the call frame)

                    output.content(
                        instance,
                        [&, position](OutputContext &oc) {
                            renderFrameFree(instance, position, oc);

                            oc.leave();
                        }
                    );
                }
            );
        };

        Block *selected_p {nullptr};

        for (auto &block_p: closure_p->blocks.blocks) {
            if (
                forkTry([&]() {
                    do_build(*block_p);
                })
            ) {
                if (selected_p) {
                    throw ErrorOverloadAmbiguous {};
                } else {
                    selected_p = block_p.get();
                }
            }
        }

        if (selected_p) {
            do_build(*selected_p);
        } else {
            throw ErrorOverloadNotFound {};
        }
    } else {
        // TODO: object as callee
        //           return (func point to error)
        //           yield (func point to next label)
        //           reset (func point to func entry)
        throw ErrorCallNotAllowed {};
    }
}

NodeCall::NodeCall(
    Node *_source, FrameMode _mode,
    std::vector<std::unique_ptr<Node>> &&_args
):
    source_p {_source},
    mode {_mode},
    args {std::move(_args)} {}

void NodeCall::buildProc(
    Instance &instance,
    Output &output
) {
    build(
        instance, output,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_proc)) {
                throw ErrorDiscardNotAllowed {};
            }
        },
        [](Instance &, size_t) {
            // nothing
        },
        [](Instance &, size_t) {
            // nothing
        }
    );
}

Type &NodeCall::buildOut(
    Instance &instance,
    Output &output,
    std::function<std::string (Type &)> &&target
) {
    Type *type_p {nullptr}; // return value

    build(
        instance, output,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_out)) {
                throw ErrorReadNotAllowed {};
            }
        },
        [](Instance &, size_t) {
            // nothing
        },
        [&](Instance &callee, size_t position) {
            Type &type {
                callee.at("result")
            };
            type_p = &type;

            // render

            output.content(
                instance,
                [&, position, target = std::move(target)](OutputContext &oc) {
                    oc.endl();
                    oc.os << target(type) << " = "
                          << instance.strInner(position) << "->result;";
                }
            );
        }
    );

    return *type_p;
}

void NodeCall::buildIn(
    Instance &instance, Type &type,
    Output &output,
    std::function<std::string (Type &)> &&target
) {
    build(
        instance, output,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_in)) {
                throw ErrorWriteNotAllowed {};
            }
        },
        [&](Instance &callee, size_t position) {
            callee.insert("input", type);

            // render

            output.content(
                instance,
                [&, position, target = std::move(target)](OutputContext &oc) {
                    oc.endl();
                    oc.os << instance.strInner(position) << "->input = "
                          << target(type) << ";";
                }
            );
        },
        [](Instance &, size_t) {
            // nothing
        }
    );
}
