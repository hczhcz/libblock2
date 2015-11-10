#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

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
        callee->buildOut(
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
            [&](Instance &child, Block &block, Instance &parent) {
                // parent

                child.insert("parent", parent);

                // render (alloc the call frame)

                OutputContext &oc {output.content(instance)};

                oc.endl();
                oc.os << "/* call */";
                oc.enter();

                switch (mode) {
                    case FrameMode::static_global:
                        oc.endl();
                        oc.os << "static " << strFrame(instance) << " "
                              << strObject(instance) << ";";
                        oc.endl();
                        oc.os << "inner = (struct frame *) &"
                              << strObject(instance) << ";";
                        break;

                    case FrameMode::static_local:
                        oc.endl();
                        oc.os << strFrame(instance) << " "
                              << strObject(instance) << ";";
                        oc.endl();
                        oc.os << "inner = (struct frame *) &"
                              << strObject(instance) << ";";
                        break;

                    case FrameMode::dynamic_stack:
                        oc.endl();
                        oc.os << "inner = alloca(sizeof("
                              << strFrame(instance)
                              << "));";
                        break;

                    case FrameMode::dynamic_gc:
                        oc.endl();
                        oc.os << "inner = GC_malloc(sizeof("
                              << strFrame(instance)
                              << "));";
                        break;

                    case FrameMode::dynamic_free:
                        oc.endl();
                        oc.os << "inner = malloc(sizeof("
                              << strFrame(instance)
                              << "));";
                        break;
                }

                // render (parent)

                oc.endl();
                oc.os << strInner(instance) << "->parent = "
                      << parent.strCast("tmp") << ";";

                // input

                before(child);

                // render (load the callee)

                oc.endl();
                oc.os << "inner->outer = callee;";
                oc.endl();
                oc.os << "callee = inner;";

                // in args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.inArg(
                        instance, child,
                        i, args[i],
                        output, strCallee(instance)
                    );
                }
            },
            [&](Instance &child, Block &block) {
                // render (header)

                OutputContext &och {output.header(instance)};

                och.endl();
                och.os << "typedef " << child.strStruct()
                       << " " << strFrame(instance) << ";";

                // render (call)

                OutputContext &oc {output.content(instance)};

                oc.endl();
                oc.os << "self->func = &&"
                      << strLabel(instance) << ";";
                // notice: reset callee->func
                oc.endl();
                oc.os << "callee->func" << " = &&"
                      << child.strFunc() << ";";

                oc.endl();
                oc.os << "callee->caller" << " = self;";
                oc.endl();
                oc.os << "self = callee;";

                oc.endl();
                oc.os << "goto *callee->func;";
                oc.endl();
                oc.os << strLabel(instance) << ":";

                oc.endl();
                oc.os << "callee = self;";
                oc.endl();
                oc.os << "self = callee->caller;";

                // out args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        instance, child,
                        i, args[i],
                        output, strCallee(instance)
                    );
                }

                // render (unload the callee)

                oc.endl();
                oc.os << "inner = callee;";
                oc.endl();
                oc.os << "callee = inner->outer;";

                // result

                after(child);

                // render (free the call frame)

                oc.leave();

                switch (mode) {
                    case FrameMode::static_global:
                    case FrameMode::static_local:
                        break;

                    case FrameMode::dynamic_stack:
                        oc.endl();
                        oc.os << "alloca(-sizeof(" << strFrame(instance) << "));";
                        break;

                    case FrameMode::dynamic_gc:
                    case FrameMode::dynamic_free:
                        break;
                }
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
        [&](Instance &child) {
            type_p = &child.at("result");

            // render

            OutputContext &oc {output.content(instance)};

            oc.endl();
            oc.os << target << " = " << strInner(instance) << "->result;";
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
        [&](Instance &child) {
            child.insert("input", type);

            // render

            OutputContext &oc {output.content(instance)};

            oc.endl();
            oc.os << strInner(instance) << "->input = " << target << ";";
        },
        [](Instance &) {
            // nothing
        }
    );
}
