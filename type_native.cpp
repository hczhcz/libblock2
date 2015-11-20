#include "type.hpp"

template <class T>
TypeNative<T> &TypeNative<T>::get() {
    static TypeNative<T> type;

    return type;
}

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

template class TypeNative<bool>;
template class TypeNative<int64_t>;
template class TypeNative<double>;
template class TypeNative<std::string>;
