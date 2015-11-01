#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void NodeSymbol::buildProc(Instance &instance, Output &output) {
    // TODO: lookup?
    // TODO: render

    const auto &symbol = instance.symbol_types.find(path[0]); // TODO

    if (symbol != instance.symbol_types.end()) {
        // nothing
    } else {
        throw std::exception {};
    }
}

Type &NodeSymbol::buildOut(Instance &instance, Output &output) {
    // TODO: lookup?
    // TODO: render

    const auto &symbol = instance.symbol_types.find(path[0]); // TODO

    if (symbol != instance.symbol_types.end()) {
        return symbol->second;
    } else {
        throw std::exception {};
    }
}

void NodeSymbol::buildIn(Instance &instance, Type &type, Output &output) {
    // TODO: lookup?
    // TODO: render

    const auto &symbol = instance.symbol_types.find(path[0]); // TODO

    if (symbol != instance.symbol_types.end()) {
        if (symbol->second != type) {
            throw std::exception {};
        }
    } else {
        instance.symbol_types.insert({path[0], type}); // TODO
    }
}
