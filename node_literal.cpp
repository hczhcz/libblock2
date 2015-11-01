#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

template <class T>
void NodeLiteral<T>::buildProc(Instance &instance, Output &output) {
    // nothing
}

template <class T>
Type &NodeLiteral<T>::buildOut(Instance &instance, Output &output) {
    // render

    output.at(instance.tuid()).content
        << "    " << util::cType("result_" + std::to_string(nuid()), value)
        << " = " << util::cCode(value) << ";\n";

    // get type

    static TypeNative<T> type {};
    return type;
}

template <class T>
void NodeLiteral<T>::buildIn(Instance &instance, Type &type, Output &output) {
    throw std::exception {};
}

template class NodeLiteral<bool>;
template class NodeLiteral<int64_t>;
template class NodeLiteral<double>;
template class NodeLiteral<std::string>;
