#include "type.hpp"

namespace libblock {

template <class T>
TypeNative<T> &TypeNative<T>::get() {
    static TypeNative<T> type;

    return type;
}

template <>
std::string TypeNative<lb_void_t>::strDecl(
    const std::string &name
) const {
    return "lb_void_t " + name;
}

template <>
std::string TypeNative<lb_bool_t>::strDecl(
    const std::string &name
) const {
    return "lb_bool_t " + name;
}

template <>
std::string TypeNative<lb_int_t>::strDecl(
    const std::string &name
) const {
    return "lb_int_t " + name;
}

template <>
std::string TypeNative<lb_real_t>::strDecl(
    const std::string &name
) const {
    return "lb_real_t " + name;
}

template <>
std::string TypeNative<std::string>::strDecl(
    const std::string &name
) const {
    return "const char *" + name;
}

template <>
std::string TypeNative<lb_void_t>::strReint() const {
    return "(*(lb_void_t *) &tmp)";
}

template <>
std::string TypeNative<lb_bool_t>::strReint() const {
    return "(*(lb_bool_t *) &tmp)";
}

template <>
std::string TypeNative<lb_int_t>::strReint() const {
    return "(*(lb_int_t *) &tmp)";
}

template <>
std::string TypeNative<lb_real_t>::strReint() const {
    return "(*(lb_real_t *) &tmp)";
}

template <>
std::string TypeNative<std::string>::strReint() const {
    return "(*(const char **) &tmp)";
}

template class TypeNative<lb_void_t>;
template class TypeNative<lb_bool_t>;
template class TypeNative<lb_int_t>;
template class TypeNative<lb_real_t>;
template class TypeNative<std::string>;

}
