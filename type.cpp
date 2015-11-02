#include "type.hpp"

uintptr_t Type::tuid() const {
    return (uintptr_t) this;
}

template <>
std::string TypeNative<bool>::renderDecl(std::string &&name) const {
    return "bool " + name;
}

template <>
std::string TypeNative<int64_t>::renderDecl(std::string &&name) const {
    return "int64_t " + name;
}

template <>
std::string TypeNative<double>::renderDecl(std::string &&name) const {
    return "double " + name;
}

template <>
std::string TypeNative<std::string>::renderDecl(std::string &&name) const {
    return "const char " + name + "[]"; // TODO: ptr?
}

std::string TypeBlock::renderDecl(std::string &&name) const {
    return "obj_" + std::to_string(parent.tuid()) + " *" + name;
}

std::string Instance::renderDecl(std::string &&name) const {
    return "obj_" + std::to_string(tuid()) + " *" + name;
}
