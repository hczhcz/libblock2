#include "type.hpp"

uintptr_t Type::tuid() const {
    return (uintptr_t) this;
}

template <>
void TypeNative<bool>::renderDecl(
    std::ostringstream &os,
    std::string &&name
) const {
    os << "bool " << name;
}

template <>
void TypeNative<int64_t>::renderDecl(
    std::ostringstream &os,
    std::string &&name
) const {
    os << "int64_t " << name;
}

template <>
void TypeNative<double>::renderDecl(
    std::ostringstream &os,
    std::string &&name
) const {
    os << "double " << name;
}

template <>
void TypeNative<std::string>::renderDecl(
    std::ostringstream &os,
    std::string &&name
) const {
    os << "const char " << name << "[]"; // TODO: ptr?
}

void TypeBlock::renderDecl(
    std::ostringstream &os,
    std::string &&name
) const {
    os << "struct obj_" << parent.tuid() << " *" << name;
}
