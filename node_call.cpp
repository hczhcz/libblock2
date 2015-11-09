#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::string NodeCall::strFrame() const { // TODO: actually not unique
    return "frame_" + std::to_string(nuid());
}

std::string NodeCall::strObject() const {
    return "object_" + std::to_string(nuid());
}

std::string NodeCall::strLabel() const {
    return "label_" + std::to_string(nuid());
}

std::string NodeCall::strInner() const {
    return "((" + strFrame() + " *) inner)";
}

std::string NodeCall::strCallee() const {
    return "((" + strFrame() + " *) callee)";
}

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special symbols:
    //     lookup: self, input, result, parent
    //     control flow: func, outer, caller

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
                        oc.os << "static " << strFrame() << " " << strObject() << ";";
                        oc.endl();
                        oc.os << "inner = &" << strObject() << ";";
                        break;

                    case FrameMode::static_local:
                        oc.endl();
                        oc.os << strFrame() << " " << strObject() << ";";
                        oc.endl();
                        oc.os << "inner = &" << strObject() << ";";
                        break;

                    case FrameMode::dynamic_stack:
                        oc.endl();
                        oc.os << "inner = alloca(sizeof(" << strFrame() << "));";
                        break;

                    case FrameMode::dynamic_gc:
                        oc.endl();
                        oc.os << "inner = GC_malloc(sizeof(" << strFrame() << "));";
                        break;

                    case FrameMode::dynamic_free:
                        oc.endl();
                        oc.os << "inner = malloc(sizeof(" << strFrame() << "));";
                        break;
                }

                // render (parent)

                oc.endl();
                oc.os << strInner() << "->parent = " << parent.strCast("tmp") << ";";

                // input

                before(child);

                // render (load the callee)

                oc.endl();
                oc.os << strInner() << "->outer = callee;";
                oc.endl();
                oc.os << "callee = inner;";

                // in args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.inArg(
                        instance, child,
                        i, args[i],
                        output, strCallee()
                    );
                }
            },
            [&](Instance &child, Block &block) {
                // render (header)

                OutputContext &och {output.header(instance)};

                och.endl();
                och.os << "typedef " << child.strStruct()
                       << " " << strFrame() << ";";

                // render (call)

                OutputContext &oc {output.content(instance)};

                oc.endl();
                oc.os << instance.strCast("self") << "->func = &&"
                      << strLabel() << ";";
                // notice: reset callee->func
                oc.endl();
                oc.os << strCallee() << "->func" << " = &&"
                      << child.strFunc() << ";";

                oc.endl();
                oc.os << strCallee() << "->caller" << " = self;";
                oc.endl();
                oc.os << "self = callee;";

                oc.endl();
                oc.os << "goto **(void ***) callee;";
                oc.endl();
                oc.os << strLabel() << ":";

                oc.endl();
                oc.os << "callee = self;";
                oc.endl();
                oc.os << "self = " << strCallee() << "->caller;";

                // out args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        instance, child,
                        i, args[i],
                        output, strCallee()
                    );
                }

                // render (unload the callee)

                oc.endl();
                oc.os << "inner = callee;";
                oc.endl();
                oc.os << "callee = " << strInner() << "->outer;";

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
                        oc.os << "alloca(-sizeof(" << strFrame() << "));";
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
            oc.os << target << " = " << strInner() << "->result;";
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
            oc.os << strInner() << "->input = " << target << ";";
        },
        [](Instance &) {
            // nothing
        }
    );
}
