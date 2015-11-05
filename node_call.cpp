#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

void NodeCall::build(
    Instance &instance, Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // special args: input, result, self, parent
    // TODO: render
    // get callee

    Type &callee_type {callee->buildOut(instance, output)};

    if (
        TypeBlock *callee_p {
            dynamic_cast<TypeBlock *>(&callee_type)
        }
    ) {
        // make call

        std::unique_ptr<Instance> child_p {
            callee_p->block.initInstance(
                callee_p->parent
            )
        };

        // in

        before(*child_p);
        for (size_t i = 0; i < args.size(); ++i) {
            callee_p->block.inArg(instance, *child_p, i, args[i], output);
        }

        Instance &child {
            callee_p->block.matchInstance(
                std::move(child_p), output
            )
        };

        // out

        for (size_t i = 0; i < args.size(); ++i) {
            callee_p->block.outArg(instance, child, i, args[i], output);
        }
        after(child);
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
