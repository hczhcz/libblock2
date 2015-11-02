#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void Block::buildProc(Instance &instance, Output &output) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // get type

    Type &type {*this};

    // render

    uintptr_t id {tuid()};

    output.at(instance.tuid()).content
        << "    " << type.renderDecl(nuidOut())
        << " = " << util::cCode(id) << ";\n";

    // return

    return type;
}

void Block::buildIn(Instance &instance, Type &type, Output &output) {
    throw std::exception {};
}
