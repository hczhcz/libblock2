#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

template <class Before, class After>
void NodeCall::build(Instance &instance, Output &output, Before &&before, After &&after) {
    // TODO: special args: input, result, self, parent
    // TODO: render

    Type &callee_type {callee->buildOut(instance, output)};

    if (
        Block *block_p {
            dynamic_cast<Block *>(&callee_type)
        }
    ) {
        if (args.size() != block_p->params.size()) {
            // error: arg size != param size
            throw std::exception {};
        }

        Instance a_instance {*block_p};

        before(a_instance);

        // input arguments
        for (size_t i = 0; i < args.size(); ++i) {
            if (block_p->params[i].second != SymbolMode::out) { // mode: in, var
                a_instance.insert(
                    block_p->params[i].first,
                    args[i]->buildOut(instance, output)
                );
            }
        }

        Instance &f_instance {
            block_p->getInstance(std::move(a_instance), output)
        };

        // output arguments
        for (size_t i = 0; i < args.size(); ++i) {
            if (block_p->params[i].second != SymbolMode::in) { // mode: out, var
                args[i]->buildIn(
                    instance,
                    f_instance.at(block_p->params[i].first),
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
