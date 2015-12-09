#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

void NodeCall::renderLabelDef(
    OutputContext &och,
    const std::string &label
) const {
    och.endl();
    och.os << "LB_FUNC(" << label << ");";
}

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
            oc.os << "static _Threal_local "
                  << instance.strCalleeType(position) << " "
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

void NodeCall::renderCall(
    OutputContext &oc,
    const std::string &label,
    const std::string &func
) const {
    oc.endl();
    oc.os << "self->func = &" << label << ";";

    // notice: reset callee->func
    oc.endl();
    oc.os << "callee->func = &" << func << ";";

    oc.endl();
    oc.os << "callee->caller = self;";
    oc.endl();
    oc.os << "self = callee;";

    oc.endl();
    oc.os << "LB_YIELD(" << label << ")";
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
        source.buildOut(
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

        // render function definition

        output.header(
            instance,
            [&, position](OutputContext &och) {
                renderLabelDef(
                    och,
                    instance.strLabel(position)
                );
            }
        );

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
                },
                [&](Instance &callee) {
                    instance.addCallee(position, callee);

                    // render (call)

                    output.content(
                        instance,
                        [&, position](OutputContext &oc) {
                            renderCall(
                                oc,
                                instance.strLabel(position),
                                callee.strFunc()
                            );
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

        for (Block &block: closure_p->blocks.blocks) {
            if (
                forkTry([&]() {
                    do_build(block);
                })
            ) {
                if (selected_p) {
                    throw ErrorOverloadAmbiguous {};
                } else {
                    selected_p = &block;
                }
            }
        }

        if (selected_p) {
            do_build(*selected_p);
        } else if (closure_p->blocks.blocks.size() == 1) {
            do_build(closure_p->blocks.blocks.front());
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
    Node &_source, FrameMode _mode,
    std::gc_vector<std::reference_wrapper<Node>> &&_args
):
    source {_source},
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

}
