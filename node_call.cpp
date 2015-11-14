#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

void NodeCall::renderFrameAlloc(
    Instance &instance, OutputContext &oc
) const {
    switch (mode) {
        case FrameMode::static_global:
            oc.endl();
            oc.os << "static "
                  << instance.strCalleeType(*this) << " "
                  << instance.strCalleeName(*this) << ";";
            oc.endl();
            oc.os << "inner = (struct frame *) &"
                  << instance.strCalleeName(*this) << ";";
            break;

        case FrameMode::static_local:
            oc.endl();
            oc.os << instance.strCalleeType(*this) << " "
                  << instance.strCalleeName(*this) << ";";
            oc.endl();
            oc.os << "inner = (struct frame *) &"
                  << instance.strCalleeName(*this) << ";";
            break;

        case FrameMode::dynamic_stack:
            oc.endl();
            oc.os << "inner = alloca(sizeof("
                  << instance.strCalleeType(*this)
                  << "));";
            break;

        case FrameMode::dynamic_gc:
            oc.endl();
            oc.os << "inner = GC_malloc(sizeof("
                  << instance.strCalleeType(*this)
                  << "));";
            break;

        case FrameMode::dynamic_free:
            oc.endl();
            oc.os << "inner = malloc(sizeof("
                  << instance.strCalleeType(*this)
                  << "));";
            break;
    }
}

void NodeCall::renderFrameFree(
    Instance &instance, OutputContext &oc
) const {
    switch (mode) {
        case FrameMode::static_global:
        case FrameMode::static_local:
            break;

        case FrameMode::dynamic_stack:
            oc.endl();
            oc.os << "alloca(-sizeof("
                  << instance.strCalleeType(*this)
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
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special symbols:
    //     lookup: self, input, result, parent
    //     control flow: frame (func, outer, caller)

    // get callee

    Type &callee_type {
        source_p->buildOut(
            instance,
            output,
            []() -> std::string {
                return "tmp";
            }
        )
    };

    if (
        TypeClosure *closure_p {
            dynamic_cast<TypeClosure *>(&callee_type)
        }
    ) {
        // callee is closure
        // create a frame and call the function

        Instance &parent {closure_p->parent};
        Block &block {*closure_p->blocks.blocks[0]};

        block.build(
            output,
            [&](Instance &callee) {
                // render (alloc the call frame)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << "/* call */";
                        oc.enter();

                        renderFrameAlloc(instance, oc);
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
                        [&](OutputContext &oc) {
                            oc.endl();
                            oc.os << instance.strInner(*this) << "->parent = "
                                  << parent.strCast("tmp") << ";";
                        }
                    );
                }

                // input

                before(callee);

                // render (load the callee)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << "inner->outer = callee;";
                        oc.endl();
                        oc.os << "callee = inner;";
                    }
                );

                // in args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.inArg(
                        instance, callee,
                        i, args[i],
                        output,
                        [&]() {
                            return instance.strCallee(*this);
                        }
                    );
                }

                // render (before call)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << "self->func = &&"
                              << instance.strLabel(*this) << ";";

                        oc.endl();
                        oc.os << "callee->caller = self;";
                        oc.endl();
                        oc.os << "self = callee;";
                    }
                );
            },
            [&](Instance &callee) {
                instance.addCallee(*this, callee);

                // render (after call)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        // notice: reset self->func
                        oc.endl();
                        oc.os << "self->func = &&"
                              << callee.strFunc() << ";";
                        oc.endl();
                        oc.os << "goto *self->func;";

                        oc.endl();
                        oc.os << instance.strLabel(*this) << ":";

                        oc.endl();
                        oc.os << "callee = self;";
                        oc.endl();
                        oc.os << "self = callee->caller;";
                    }
                );

                // out args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        instance, callee,
                        i, args[i],
                        output,
                        [&]() {
                            return instance.strCallee(*this);
                        }
                    );
                }

                // render (unload the callee)

                output.content(
                    instance,
                    [&](OutputContext &oc) {

                        oc.endl();
                        oc.os << "inner = callee;";
                        oc.endl();
                        oc.os << "callee = inner->outer;";
                    }
                );

                // result

                after(callee);

                // render (free the call frame)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        renderFrameFree(instance, oc);

                        oc.leave();
                    }
                );
            }
        );
    } else {
        // TODO: object as callee
        //           return (func point to error)
        //           yield (func point to next label)
        //           reset (func point to func entry)
        throw ErrorCallNotAllowed {};
    }
}

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
        [](Instance &) {
            // nothing
        },
        [](Instance &) {
            // nothing
        }
    );
}

Type &NodeCall::buildOut(
    Instance &instance,
    Output &output,
    std::function<std::string ()> &&target
) {
    Type *type_p {nullptr}; // return value

    build(
        instance, output,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_out)) {
                throw ErrorReadNotAllowed {};
            }
        },
        [](Instance &) {
            // nothing
        },
        [&](Instance &callee) {
            type_p = &callee.at("result");

            // render

            output.content(
                instance,
                [&, target = std::move(target)](OutputContext &oc) {
                    oc.endl();
                    oc.os << target() << " = "
                          << instance.strInner(*this) << "->result;";
                }
            );
        }
    );

    return *type_p;
}

void NodeCall::buildIn(
    Instance &instance, Type &type,
    Output &output,
    std::function<std::string ()> &&target
) {
    build(
        instance, output,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_in)) {
                throw ErrorWriteNotAllowed {};
            }
        },
        [&](Instance &callee) {
            callee.insert("input", type);

            // render

            output.content(
                instance,
                [&, target = std::move(target)](OutputContext &oc) {
                    oc.endl();
                    oc.os << instance.strInner(*this) << "->input = "
                          << target() << ";";
                }
            );
        },
        [](Instance &) {
            // nothing
        }
    );
}
