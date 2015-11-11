#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

std::string NodeCall::strFrame(Instance &instance) const {
    return "frame_"
        + std::to_string(instance.tuid()) + "_"
        + std::to_string(nuid());
}

std::string NodeCall::strObject(Instance &instance) const {
    return "object_"
        + std::to_string(instance.tuid()) + "_"
        + std::to_string(nuid());
}

std::string NodeCall::strLabel(Instance &instance) const {
    return "label_"
        + std::to_string(instance.tuid()) + "_"
        + std::to_string(nuid());
}

std::string NodeCall::strInner(Instance &instance) const {
    return "((" + strFrame(instance) + " *) inner)";
}

std::string NodeCall::strCallee(Instance &instance) const {
    return "((" + strFrame(instance) + " *) callee)";
}

void NodeCall::build(
    Instance &instance, Output &output,
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
            output, "tmp"
        )
    };

    if (
        TypeClosure *closure_p {
            dynamic_cast<TypeClosure *>(&callee_type)
        }
    ) {
        // callee is closure
        // create a frame and call the function

        closure_p->call(
            output,
            [&](Instance &callee, Block &block, Instance &parent) {
                // parent

                callee.insert("parent", parent);

                // render (alloc the call frame)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << "/* call */";
                        oc.enter();

                        switch (mode) {
                            case CallMode::static_global:
                                oc.endl();
                                oc.os << "static " << strFrame(instance) << " "
                                      << strObject(instance) << ";";
                                oc.endl();
                                oc.os << "inner = (struct frame *) &"
                                      << strObject(instance) << ";";
                                break;

                            case CallMode::static_local:
                                oc.endl();
                                oc.os << strFrame(instance) << " "
                                      << strObject(instance) << ";";
                                oc.endl();
                                oc.os << "inner = (struct frame *) &"
                                      << strObject(instance) << ";";
                                break;

                            case CallMode::dynamic_stack:
                                oc.endl();
                                oc.os << "inner = alloca(sizeof("
                                      << strFrame(instance)
                                      << "));";
                                break;

                            case CallMode::dynamic_gc:
                                oc.endl();
                                oc.os << "inner = GC_malloc(sizeof("
                                      << strFrame(instance)
                                      << "));";
                                break;

                            case CallMode::dynamic_free:
                                oc.endl();
                                oc.os << "inner = malloc(sizeof("
                                      << strFrame(instance)
                                      << "));";
                                break;
                        }
                    }
                );

                // render (parent)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << strInner(instance) << "->parent = "
                              << parent.strCast("tmp") << ";";
                    }
                );

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
                        output, strCallee(instance)
                    );
                }
            },
            [&](Instance &callee, Block &block) {
                // render (header) // TODO: remove

                output.header(
                    instance,
                    [&](OutputContext &och) {
                        och.endl();
                        och.os << "typedef " << callee.strStruct()
                               << " " << strFrame(instance) << ";";
                    }
                );

                // render (call)

                output.content(
                    instance,
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << "self->func = &&"
                              << strLabel(instance) << ";";
                        // notice: reset callee->func
                        oc.endl();
                        oc.os << "callee->func" << " = &&"
                              << callee.strFunc() << ";";

                        oc.endl();
                        oc.os << "callee->caller" << " = self;";
                        oc.endl();
                        oc.os << "self = callee;";

                        oc.endl();
                        oc.os << "goto *self->func;";
                        oc.endl();
                        oc.os << strLabel(instance) << ":";

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
                        output, strCallee(instance)
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
                        switch (mode) {
                            case CallMode::static_global:
                            case CallMode::static_local:
                                break;

                            case CallMode::dynamic_stack:
                                oc.endl();
                                oc.os << "alloca(-sizeof("
                                      << strFrame(instance)
                                      << "));";
                                break;

                            case CallMode::dynamic_gc:
                            case CallMode::dynamic_free:
                                break;
                        }

                        oc.leave();
                    }
                );
            }
        );
    } else {
        // error: value as callee // TODO
        throw ErrorCallNotAllowed {};
    }
}

void NodeCall::buildProc(
    Instance &instance,
    Output &output
) {
    build(
        instance, output,
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
    Output &output, const std::string &target
) {
    Type *type_p {nullptr}; // return value

    build(
        instance, output,
        [](Instance &) {
            // nothing
        },
        [&](Instance &callee) {
            type_p = &callee.at("result");

            // render

            output.content(
                instance,
                [&, target](OutputContext &oc) {
                    oc.endl();
                    oc.os << target << " = "
                          << strInner(instance) << "->result;";
                }
            );
        }
    );

    return *type_p;
}

void NodeCall::buildIn(
    Instance &instance, Type &type,
    Output &output, const std::string &target
) {
    build(
        instance, output,
        [&](Instance &callee) {
            callee.insert("input", type);

            // render

            output.content(
                instance,
                [&, target](OutputContext &oc) {
                    oc.endl();
                    oc.os << strInner(instance) << "->input = "
                          << target << ";";
                }
            );
        },
        [](Instance &) {
            // nothing
        }
    );
}
