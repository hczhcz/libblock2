#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::string NodeCall::nuidFrame() const {
    return "frame_" + std::to_string(nuid()); // TODO
}

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special args: input, result, self, parent

    // get callee
    Type &callee_type {
        callee->buildOut(
            instance,
            output, nuidFrame() + "->parent"
        )
    };

    if (
        TypeBlock *callee_p {
            dynamic_cast<TypeBlock *>(&callee_type)
        }
    ) {
        // call
        callee_p->call(
            output,
            [&](Instance &child, Block &block) {
                // render (before call)

                std::ostream &osh {output.osHeader(instance)};

                osh << child.decl(nuidFrame()) << ";\n";

                // in

                before(child);

                for (size_t i = 0; i < args.size(); ++i) {
                    block.inArg(
                        instance, child,
                        i, args[i],
                        output, nuidFrame()
                    );
                }
            },
            [&](Instance &child, Block &block) {
                // render (call)

                std::ostream &os {output.osContent(instance)};

                child.renderFuncCall(os, nuidFrame());

                // out

                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        instance, child,
                        i, args[i],
                        output, nuidFrame()
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

            std::ostream &os {output.osContent(instance)};

            os << "    " << target << " = " << nuidFrame() << "->result;\n";
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

            std::ostream &os {output.osContent(instance)};

            os << "    " << nuidFrame() << "->input = " << target << ";\n";
        },
        [](Instance &) {
            // nothing
        }
    );
}
