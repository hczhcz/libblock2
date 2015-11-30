#pragma once

#include "myparser_ast_spec.hpp"

namespace myparser {

template <>
class Pass<PASS_HIGHLIGHT>: public PassProto<PASS_HIGHLIGHT> {
protected:
    std::ostream &out;

    virtual void putText(const std::string &text) {
        out << text;
    }

    virtual void putSpace(const std::string &text) {
        out << text;
    }

    virtual void putKeyword(const std::string &text) {
        out << text;
    }

    virtual void putData(const std::string &text) {
        out << text;
    }

    virtual void putString(const std::string &text) {
        out << text;
    }

public:
    inline Pass(
        std::ostream &target
    ): PassProto<PASS_HIGHLIGHT>(), out(target) {}

    // virtual ~Pass() {}

    // specialized nodes

    template <size_t I>
    void run(const NodeSpace<I> *node) {
        putSpace(node->getFullText());
    }

    template <size_t I>
    void run(const NodeKeyword<I> *node) {
        putKeyword(node->getFullText());
    }

    template <class T, class E>
    void run(const NodeData<T, E> *node) {
        putData(node->getFullText());
    }

    template <size_t QL, size_t QR, char EC, class E>
    void run(const NodeString<QL, QR, EC, E> *node) {
        putString(node->getFullText());
    }

    // common nodes

    template <size_t I>
    void run(const NodeListIndexed<I> *node) {
        for (const Node<> *child: node->getChildren()) {
            child->runPass(this);
        }
    }

    template <class TX = void> // actually not a template
    void run(const NodeTextPure<> *node) {
        putText(node->getFullText());
    }

    template <class E>
    void run(const NodeTextOrError<E> *node) {
        if (node->accepted()) {
            putText(node->getFullText());
        }
    }

    template <class E>
    void run(const NodeError<E> *) {}
};

template <class TX = void> // actually not a template
class PassHighlight: public Pass<PASS_HIGHLIGHT> { // console ver
protected:
    virtual void putSpace(const std::string &text) {
        out << style_space << text << style_normal;
    }

    virtual void putKeyword(const std::string &text) {
        out << style_keyword << text << style_normal;
    }

    virtual void putData(const std::string &text) {
        out << style_data << text << style_normal;
    }

    virtual void putString(const std::string &text) {
        out << style_string << text << style_normal;
    }

public:
    inline PassHighlight(
        std::ostream &target
    ): Pass<PASS_HIGHLIGHT>(target) {}

    // virtual ~PassHighlight() {}
};

}
