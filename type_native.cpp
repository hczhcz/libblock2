#include "type.hpp"

template <>
std::string TypeNative<bool>::strDecl(
    const std::string &name
) const {
    return "bool " + name;
}

template <>
std::string TypeNative<int64_t>::strDecl(
    const std::string &name
) const {
    return "int64_t " + name;
}

template <>
std::string TypeNative<double>::strDecl(
    const std::string &name
) const {
    return "double " + name;
}

template <>
std::string TypeNative<std::string>::strDecl(
    const std::string &name
) const {
    return "const char *" + name;
}
