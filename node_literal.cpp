#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

template <>
void NodeLiteral<bool>::renderValue(std::ostringstream &os) const {
    os << (value ? "true" : "false");
}

template <>
void NodeLiteral<int64_t>::renderValue(std::ostringstream &os) const {
    os << value;
}

template <>
void NodeLiteral<double>::renderValue(std::ostringstream &os) const {
    // return std::to_string(value);
    os << "*(double *) (uint64_t []) {" << *(uint64_t *) &value << "}";
}

template <>
void NodeLiteral<std::string>::renderValue(std::ostringstream &os) const {
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

    std::ostringstream &os {output.at(instance).content};

    os << "    ";
    type.renderDecl(os, nuidOut());
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

template struct NodeLiteral<bool>;
template struct NodeLiteral<int64_t>;
template struct NodeLiteral<double>;
template struct NodeLiteral<std::string>;
