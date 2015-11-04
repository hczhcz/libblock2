#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

template <class Before, class After>
void NodeCall::build(
    Instance &instance, Output &output,
    Before &&before, After &&after
) {
    // special args: input, result, self, parent
    // TODO: render

    Type &callee_type {callee->buildOut(instance, output)};

    if (
        TypeBlock *callee_p {
            dynamic_cast<TypeBlock *>(&callee_type)
        }
    ) {
        std::unique_ptr<Instance> a_instance {
            callee_p->block.initInstance(
                callee_p->parent
            )
        };

        before(*a_instance);
        for (size_t i = 0; i < args.size(); ++i) {
            callee_p->block.inArg(instance, *a_instance, i, args[i], output);
        }

        Instance &f_instance {
            callee_p->block.matchInstance(
                std::move(a_instance), output
            )
        };

        for (size_t i = 0; i < args.size(); ++i) {
            callee_p->block.outArg(instance, f_instance, i, args[i], output);
        }
        after(f_instance);
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
        [&](Instance &instance) {
            type_p = &instance.at("result");
        }
    );

    return *type_p;
}

void NodeCall::buildIn(Instance &instance, Type &type, Output &output) {
    build(
        instance, output,
        [&](Instance &instance) {
            instance.insert("input", type);
        },
        [](Instance &) {
            // nothing
        }
    );
}
