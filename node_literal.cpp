#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

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
    // nothing
}

template <class T>
Type &NodeLiteral<T>::buildOut(
    Instance &instance,
    Output &output, const std::string &target
) {
    // get type

    static TypeNative<T> type;

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << target << " = ";
    renderValue(oc.os);
    oc.os << ";";

    // return

    return type;
}

template <class T>
void NodeLiteral<T>::buildIn(
    Instance &, Type &,
    Output &, const std::string &
) {
    throw ErrorWriteNotAllowed {};
}

template class NodeLiteral<bool>;
template class NodeLiteral<int64_t>;
template class NodeLiteral<double>;
template class NodeLiteral<std::string>;
