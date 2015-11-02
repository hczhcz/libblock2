#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void NodeSymbol::buildProc(Instance &instance, Output &output) {
    // TODO: lookup?
    // TODO: render

    instance.lookup(path[0]); // TODO
}

Type &NodeSymbol::buildOut(Instance &instance, Output &output) {
    // TODO: lookup?
    // TODO: render

    return instance.lookup(path[0]);
}

void NodeSymbol::buildIn(Instance &instance, Type &type, Output &output) {
    // TODO: lookup?
    // TODO: render

    instance.lookup(
        path[0],
        [&](Type &symbol_type) {
            if (symbol_type != type) {
                throw std::exception {};
            }
        },
        [&]() {
            instance.symbol_types.insert({path[0], type}); // TODO
        }
    );
}
