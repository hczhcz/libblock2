#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::string NodeCall::strFrame() const {
    return "frame_" + std::to_string(nuid());
}

std::string NodeCall::strCallee() const {
    return "((" + strFrame() + " *) callee)";
}

std::string NodeCall::strLabel() const {
    return "label_" + std::to_string(nuid());
}

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special symbols:
    //     lookup: self, input, result, parent
    //     control flow: func, caller

    // render (init call frame)

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << "callee = malloc(sizeof(" << strFrame() << "));"; // TODO: GC?

    oc.endl();
    oc.os << "/* call */";
    oc.enter();

    // get callee
    // notice: "callee" here is actually "closure"
    //         this may change if value could be callee

    Type &callee_type {
        callee->buildOut(
            instance,
            output, strCallee() + "->parent"
        )
    };

    if (
        TypeClosure *closure_p {
            dynamic_cast<TypeClosure *>(&callee_type)
        }
    ) {
        // call

        closure_p->call(
            output,
            [&](Instance &child, Block &block) {
                // in

                before(child);

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

                oc.endl();
                oc.os << instance.strCast() << "->func = &&"
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

                // out

                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        instance, child,
                        i, args[i],
                        output, strCallee()
                    );
                }

                after(child);
            }
        );
    } else {
        // error: value as callee
        throw ErrorCallNotAllowed {};
    }

    // render (exit)

    oc.leave();
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
            oc.os << target << " = " << strCallee() << "->result;";
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
            oc.os << strCallee() << "->input = " << target << ";";
        },
        [](Instance &) {
            // nothing
        }
    );
}
