#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void Block::buildProc(Instance &instance, Output &output) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // get type

    instance.children.push_back({instance, *this});
    Type &type {instance.children.back()};

    // render

    output.at(instance.tuid()).content
        << "    " << type.renderDecl(nuidOut())
        << " = self;\n";

    // return

    return type;
}

void Block::buildIn(Instance &instance, Type &type, Output &output) {
    throw std::exception {};
}
