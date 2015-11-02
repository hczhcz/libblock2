#include "output.hpp"
#include "node.hpp"

template <class Before, class After>
void NodeCall::build(Instance &instance, Output &output, Before &&before, After &&after) {
    // special args: input, result, self, parent, caller
    // TODO: parent, caller
    // TODO: render

    Type &callee_type {callee->buildOut(instance, output)};

    if (
        TypeBlock *callee_p {
            dynamic_cast<TypeBlock *>(&callee_type)
        }
    ) {
        if (args.size() != callee_p->block.params.size()) {
            // error: arg size != param size
            throw std::exception {};
        }

        Instance a_instance {callee_p->block};

        a_instance.insert("parent", callee_p->parent);

        before(a_instance);

        // input arguments
        for (size_t i = 0; i < args.size(); ++i) {
            if (callee_p->block.params[i].second != SymbolMode::out) { // mode: in, var
                a_instance.insert(
                    callee_p->block.params[i].first,
                    args[i]->buildOut(instance, output)
                );
            }
        }

        Instance &f_instance {
            callee_p->block.getInstance(std::move(a_instance), output)
        };

        // output arguments
        for (size_t i = 0; i < args.size(); ++i) {
            if (callee_p->block.params[i].second != SymbolMode::in) { // mode: out, var
                args[i]->buildIn(
                    instance,
                    f_instance.at(callee_p->block.params[i].first),
                    output
                );
            }
        }

        after(f_instance);
    } else {
        // error: value as callee
        throw std::exception {};
    }
}

void NodeCall::buildProc(Instance &instance, Output &output) {
    build(
        instance, output,
        [](Instance &instance) {
            // nothing
        },
        [](Instance &instance) {
            // nothing
        }
    );
}

Type &NodeCall::buildOut(Instance &instance, Output &output) {
    Type *type_p;

    build(
        instance, output,
        [](Instance &instance) {
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
        [](Instance &instance) {
            // nothing
        }
    );
}
