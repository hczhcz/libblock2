#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void NodeSymbol::buildProc(Instance &instance, Output &output) {
    // TODO: render

    instance.lookup(path).at(name);
}

Type &NodeSymbol::buildOut(Instance &instance, Output &output) {
    // TODO: render

    return instance.lookup(path).at(name);
}

void NodeSymbol::buildIn(Instance &instance, Type &type, Output &output) {
    // TODO: render

    instance.lookup(path).insert(name, type);
}
