#pragma once

#include "include.hpp"

namespace myparser {

using ErrorId = MP_STR("Illegal identifier");
using ErrorLiteral = MP_STR("Illegal literal");

template <class E>
class NodeId: public NodeTextOrError<E> {
private:
    /* const */ bool succeed;

public:
    inline NodeId(
        const Input &input, const Input &end
    ): NodeTextOrError<E>(input, end) {
        static const std::set<std::string> keywords = {
            "div", "mod", "shl", "shr", "ushr", "rol", "ror",
            "or", "xor", "and", "not", "is", "in ", "out", "var"
        };

        succeed = keywords.find(std::string(input, end)) == keywords.cend();
    }

    // virtual ~NodeId() {}

    virtual bool accepted() const {
        return succeed;
    }
};

// specialization

template <>
class NodeBaseText<MP_STR("id")> {
public:
    template <class TX = void> // actually not a template
    using Type = NodeId<ErrorId>;
};

template <>
class NodeBaseText<MP_STR("int")> {
public:
    template <class TX = void> // actually not a template
    using Type = NodeData<int64_t, ErrorLiteral>;
};

template <>
class NodeBaseText<MP_STR("real")> {
public:
    template <class TX = void> // actually not a template
    using Type = NodeData<double, ErrorLiteral>;
};

template <>
class NodeBaseText<MP_STR("str")> {
public:
    template <class TX = void> // actually not a template
    using Type = NodeString<1, 1, '\\', ErrorLiteral>;
};

}
