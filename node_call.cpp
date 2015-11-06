#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::string NodeCall::nuidCallee() const {
    return "callee_" + std::to_string(nuid()); // TODO
}

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special args: input, result, self, parent

    // get callee
    Type &callee_type {callee->buildOut(instance, output, nuidCallee())};

    if (
        TypeBlock *callee_p {
            dynamic_cast<TypeBlock *>(&callee_type)
        }
    ) {
        // call
        callee_p->call(
            output,
            [&](Instance &child, Block &block) {
                before(child);

                for (size_t i = 0; i < args.size(); ++i) {
                    block.inArg(
                        instance, child,
                        i, args[i],
                        output, "<in?>"
                    );
                }
            },
            [&](Instance &child, Block &block) {
                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        instance, child,
                        i, args[i],
                        output, "<out?>"
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
        },
        [](Instance &) {
            // nothing
        }
    );
}
