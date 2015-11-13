#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

TypeClosure::TypeClosure(Instance &_parent, NodeBlock &_blocks):
    parent {_parent},
    blocks {_blocks} {}

std::string TypeClosure::strDecl(const std::string &name) const {
    // type: struct <parent object> *
    return parent.strDecl(name);
}
