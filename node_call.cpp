#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special args: input, result, self, parent

    // get callee
    Type &callee_type {callee->buildOut(instance, output)};

    if (
        TypeBlock *callee_p {
            dynamic_cast<TypeBlock *>(&callee_type)
        }
    ) {
        // call
        callee_p->call(
            instance, args, output,
            std::move(before),
            std::move(after)
        );
    } else {
        // error: value as callee
        throw ErrorCallNotAllowed {};
    }
}

void NodeCall::buildProc(Instance &instance, Output &output) {
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

Type &NodeCall::buildOut(Instance &instance, Output &output) {
    Type *type_p;

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

void NodeCall::buildIn(Instance &instance, Type &type, Output &output) {
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
