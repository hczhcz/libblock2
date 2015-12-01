#include "type.hpp"

namespace libblock {

TypeClosure::TypeClosure(Type &_parent, NodeBlock &_blocks):
    parent {_parent},
    blocks {_blocks} {}

std::string TypeClosure::strDecl(const std::string &name) const {
    return parent.strDecl(name);
}

std::string TypeClosure::strReint(const std::string &name) const {
    return parent.strReint(name);
}

}
