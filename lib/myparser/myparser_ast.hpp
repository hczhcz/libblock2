#pragma once

#include "myparser_str.hpp"
#include "myparser_pass.hpp"

namespace myparser {

using Input = std::string::const_iterator;

using BuiltinRoot = MP_STR("root");
using BuiltinSpace = MP_STR("space");
using BuiltinKeyword = MP_STR("keyword");
using BuiltinError = MP_STR("error");

template <class TX = void> // actually not a template
class Node {
private:
    inline Node() = delete;

    const Input pos;
    Input tail;

protected:
    inline Node(
        const Input &input
    ): pos(input), tail(input) {}

    inline void seek(const Input &end) {
        tail = end;
    }

public:
    virtual ~Node() {} // destructable (public)

    virtual void free() {
        delete this;
    }

    virtual bool empty() const = 0;

    virtual void runPass(PassBase<> *pass) const = 0;

    virtual const std::string &getRuleName() const = 0;

    // TODO: cache?
    inline const std::string getFullText() const {
        return std::string(pos, tail);
    }

    inline const Input &getPos() const {
        return pos;
    }

    inline const Input &getTail() const {
        return tail;
    }

    inline Node<> *challengeLonger(Node<> *target) {
        if (!target) {
            return this;
        }

        if (getTail() > target->getTail()) {
            target->free();
            return this;
        } else {
            this->free();
            return target;
        }
    }
};

template <class TX = void> // actually not a template
class NodeList: public Node<> {
private:
    std::vector<Node<> *> children;

    size_t basepos;
    NodeList<> *brother = nullptr;

protected:
    inline NodeList(
        const Input &input
    ): Node<>(input), children(), basepos(0) {}

public:
    virtual ~NodeList() {
        for (size_t i = basepos; i < children.size(); ++i) {
            children[i]->free();
        }
    }

    virtual void free() {
        if (brother) {
            brother->basepos = 0;
            brother->brother = nullptr;
        }
        delete this;
    }

    virtual bool empty() const {
        for (Node<> *child: children) {
            if (!child->empty()) {
                return false;
            }
        }

        return true;
    }

    inline void bind(NodeList<> *target, const size_t pos) {
        brother = target;
        basepos = pos;
    }

    inline void putChild(Node<> *value) {
        children.push_back(value);
        seek(value->getTail());
    }

    inline const std::vector<Node<> *> &getChildren() const {
        return children;
    }
};

template <size_t I>
class NodeListIndexed: public NodeList<> {
public:
    inline NodeListIndexed(
        const Input &input
    ): NodeList<>(input) {}

    // virtual ~NodeListIndexed() {}

    inline size_t getIndex() const {
        return I;
    }
};

template <class TX = void> // actually not a template
class NodeText: public Node<> {
protected:
    inline NodeText(
        const Input &input, const Input &end
    ): Node<>(input) {
        seek(end);
    }

public:
    // virtual ~NodeText() {}

    virtual bool accepted() const {
        return true;
    }

    virtual bool empty() const {
        return accepted() && (getPos() == getTail());
    }
};

template <class TX = void> // actually not a template
class NodeTextPure: public NodeText<> {
public:
    inline NodeTextPure(
        const Input &input, const Input &end
    ): NodeText<>(input, end) {}

    // virtual ~NodeTextPure() {}
};

template <class E>
class NodeTextOrError: public NodeText<> {
public:
    inline NodeTextOrError(
        const Input &input, const Input &end
    ): NodeText<>(input, end) {}

    // virtual ~NodeTextOrError() {}
};

template <class E>
class NodeError: public Node<> {
public:
    inline NodeError(
        const Input &input
    ): Node<>(input) {}

    // virtual ~NodeError() {}

    virtual bool empty() const {
        return false;
    }
};

// could specialize
template <class N>
class NodeBaseList {
public:
    template <size_t I>
    using Type = NodeListIndexed<I>;
};

// could specialize
template <class N>
class NodeBaseText {
public:
    template <class TX = void> // actually not a template
    using Type = NodeTextPure<>;
};

// could specialize
template <class N>
class NodeBaseError {
public:
    template <class E>
    using Type = NodeError<E>;
};

template <class N, class T>
class NodeTyped: public T {
public:
    using T::T;

    virtual void runPass(PassBase<> *pass) const {
        passCall(pass, this);
    }

    virtual const std::string &getRuleName() const {
        return N::getStr();
    }
};

template <class N, size_t I>
using NodeTypedList = NodeTyped<N, typename NodeBaseList<N>::template Type<I>>;

template <class N>
using NodeTypedText = NodeTyped<N, typename NodeBaseText<N>::template Type<>>;

template <class N, class E>
using NodeTypedError = NodeTyped<N, typename NodeBaseError<N>::template Type<E>>;

}
