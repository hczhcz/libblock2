#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void Block::buildProc(Instance &instance, Output &output) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // render

    uintptr_t id {tuid()};

    output.at(instance.tuid()).content
        << "    " << util::cType(nuidOut(), id)
        << " = " << util::cCode(id) << ";\n";

    // get type

    return *this;
}

void Block::buildIn(Instance &instance, Type &type, Output &output) {
    throw std::exception {};
}
