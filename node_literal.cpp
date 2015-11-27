#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

template <>
void NodeLiteral<std::nullptr_t>::renderValue(
    std::ostream &os
) const {
    os << "(struct empty) {}";
}

template <>
void NodeLiteral<bool>::renderValue(
    std::ostream &os
) const {
    os << (value ? "true" : "false");
}

template <>
void NodeLiteral<int64_t>::renderValue(
    std::ostream &os
) const {
    os << value;
}

template <>
void NodeLiteral<double>::renderValue(
    std::ostream &os
) const {
    os << "*(double *) (uint64_t []) {" << *(uint64_t *) &value << "}";
}

template <>
void NodeLiteral<std::string>::renderValue(
    std::ostream &os
) const {
    os << "(const char []) {";

    for (char i: value) {
        os << (int) i << ",";
    }

    os << "0}";
}

template <class T>
NodeLiteral<T>::NodeLiteral(T &&_value):
    value {std::move(_value)} {}

template <class T>
void NodeLiteral<T>::buildProc(
    Instance &,
    Output &
) {
    throw ErrorDiscardNotAllowed {};
}

template <class T>
Type &NodeLiteral<T>::buildOut(
    Instance &instance,
    Output &output,
    std::function<std::string (Type &)> &&target
) {
    // get type

    Type &type {
        TypeNative<T>::get()
    };

    // render

    output.content(
        instance,
        [&, target = std::move(target)](OutputContext &oc) {
            oc.endl();
            oc.os << target(type) << " = ";
            renderValue(oc.os);
            oc.os << ";";
        }
    );

    // return

    return type;
}

template <class T>
void NodeLiteral<T>::buildIn(
    Instance &, Type &,
    Output &,
    std::function<std::string (Type &)> &&
) {
    throw ErrorWriteNotAllowed {};
}

template class NodeLiteral<std::nullptr_t>;
template class NodeLiteral<bool>;
template class NodeLiteral<int64_t>;
template class NodeLiteral<double>;
template class NodeLiteral<std::string>;
