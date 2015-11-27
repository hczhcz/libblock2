#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

TypeClosure::TypeClosure(Type &_parent, NodeBlock &_blocks):
    parent {_parent},
    blocks {_blocks} {}

std::string TypeClosure::strDecl(const std::string &name) const {
    // type: struct <parent object> *
    return parent.strDecl(name);
}
