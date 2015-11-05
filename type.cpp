#include "type.hpp"

uintptr_t Type::tuid() const {
    return (uintptr_t) this;
}

template <>
void TypeNative<bool>::renderDecl(
    std::ostream &os,
    const std::string &name
) const {
    os << "bool " << name;
}

template <>
void TypeNative<int64_t>::renderDecl(
    std::ostream &os,
    const std::string &name
) const {
    os << "int64_t " << name;
}

template <>
void TypeNative<double>::renderDecl(
    std::ostream &os,
    const std::string &name
) const {
    os << "double " << name;
}

template <>
void TypeNative<std::string>::renderDecl(
    std::ostream &os,
    const std::string &name
) const {
    os << "const char *" << name;
}

void TypeBlock::renderDecl(
    std::ostream &os,
    const std::string &name
) const {
    os << "struct obj_" << parent.tuid() << " *" << name;
}
