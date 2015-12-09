#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

namespace libblock {

template <>
void NodeLiteral<lb_void_t>::renderValue(
    std::ostream &os
) const {
    os << "(lb_void_t) {}";
}

template <>
void NodeLiteral<lb_bool_t>::renderValue(
    std::ostream &os
) const {
    os << (value ? "true" : "false");
}

template <>
void NodeLiteral<lb_int_t>::renderValue(
    std::ostream &os
) const {
    os << value;
}

template <>
void NodeLiteral<lb_real_t>::renderValue(
    std::ostream &os
) const {
    os << "*(lb_real_t *) (lb_reg_t []) {" << *(lb_reg_t *) &value << "}";
}

template <>
void NodeLiteral<std::string>::renderValue(
    std::ostream &os
) const {
    os << "\"";

    for (char i: value) {
        os << "\\x"
           << "0123456789ABCDEF"[i >> 4 & 15]
           << "0123456789ABCDEF"[i & 15];
    }

    os << "\\x00\"";
}

template <class T>
NodeLiteral<T>::NodeLiteral(T &&_value):
    value {std::move(_value)} {}

template <class T>
NodeLiteral<T>::NodeLiteral(const T &_value):
    value {_value} {}

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

template class NodeLiteral<lb_void_t>;
template class NodeLiteral<lb_bool_t>;
template class NodeLiteral<lb_int_t>;
template class NodeLiteral<lb_real_t>;
template class NodeLiteral<std::string>;

}
