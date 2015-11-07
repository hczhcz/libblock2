#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::string NodeCall::strFrame() const {
    return "frame_" + std::to_string(nuid()); // TODO
}

std::string NodeCall::strMember(const std::string &name) const {
    return strFrame() + "->" + name; // TODO
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
            output, strMember("parent")
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
                // render (frame)

                std::ostream &osh {output.osHeader(instance)};

                osh << "static " << child.decl(strFrame()) << ";\n";

                // render (call)

                std::ostream &os {output.osContent(instance)};

                child.renderFuncCall(os, strFrame());

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

            std::ostream &os {output.osContent(instance)};

            os << "    " << target << " = " << strMember("result") << ";\n";
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

            os << "    " << strMember("input") << " = " << target << ";\n";
        },
        [](Instance &) {
            // nothing
        }
    );
}
