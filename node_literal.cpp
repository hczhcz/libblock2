#include "exception.hpp"
#include "output.hpp"
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

    os << "}";
}

template <class T>
void NodeLiteral<T>::buildProc(Instance &, Output &) {
    // nothing
}

template <class T>
Type &NodeLiteral<T>::buildOut(Instance &instance, Output &output) {
    // get type

    static TypeNative<T> type;

    // render

    std::ostream &os {output.osContent(instance)};

    os << "    ";
    os << type.decl(nuidOut());
    os << " = ";
    renderValue(os);
    os << ";\n";

    // return

    return type;
}

template <class T>
void NodeLiteral<T>::buildIn(Instance &, Type &, Output &) {
    throw ErrorWriteNotAllowed {};
}

template class NodeLiteral<bool>;
template class NodeLiteral<int64_t>;
template class NodeLiteral<double>;
template class NodeLiteral<std::string>;
