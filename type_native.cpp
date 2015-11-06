#include "type.hpp"

template <>
std::string TypeNative<bool>::decl(
    const std::string &name
) const {
    return "bool " + name;
}

template <>
std::string TypeNative<int64_t>::decl(
    const std::string &name
) const {
    return "int64_t " + name;
}

template <>
std::string TypeNative<double>::decl(
    const std::string &name
) const {
    return "double " + name;
}

template <>
std::string TypeNative<std::string>::decl(
    const std::string &name
) const {
    return "const char *" + name;
}
