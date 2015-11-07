#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::string NodeCall::strFrame() const {
    return "frame_" + std::to_string(nuid()); // TODO
}

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special symbols:
    //     lookup: self, input, result, parent
    //     control flow: caller, func

    // get callee
    Type &callee_type {
        callee->buildOut(
            instance,
            output, strFrame() + "->parent"
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
                        output, strFrame()
                    );
                }
            },
            [&](Instance &child, Block &block) {
                // render (call)

                OutputContext &oc {output.content(instance)};

                oc.endl(0);
                child.renderFuncCall(oc, nuid(), strFrame());

                // out

                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        instance, child,
                        i, args[i],
                        output, strFrame()
                    );
                }

                after(child);
            }
        );
    } else {
        // error: value as callee
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

            oc.endl(0);
            oc.os << target << " = " << strFrame() << "->result;";
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

            oc.endl(0);
            oc.os << strFrame() << "->input = " << target << ";";
        },
        [](Instance &) {
            // nothing
        }
    );
}
