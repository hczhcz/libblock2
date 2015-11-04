#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

template <>
std::string NodeLiteral<bool>::renderValue() {
    return value ? "true" : "false";
}

template <>
std::string NodeLiteral<int64_t>::renderValue() {
    return std::to_string(value);
}

template <>
std::string NodeLiteral<double>::renderValue() {
    // return std::to_string(value);
    return "*(double *) (uint64_t []) {"
        + std::to_string(*(uint64_t *) &value) + "}";
}

template <>
std::string NodeLiteral<std::string>::renderValue() {
    std::string result;

    result += "(const char []) {";
    for (char i: value) {
        result += std::to_string((int) i) + ",";
    }
    result += "}";

    return result;
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

    output.at(instance).content
        << "    " << type.renderDecl(nuidOut())
        << " = " << renderValue() << ";\n";

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
