#pragma once

#include "myparser_ast_spec.hpp"

namespace myparser {

template <>
class Pass<PASS_REPR>: public PassProto<PASS_REPR> {
private:
    const bool optionV;
    const bool optionC;

protected:
    std::ostream &out;
    size_t indent;

    virtual void putName(const std::string &) {}

    virtual void putIndex(const size_t) {}

    virtual void putText(const std::string &) {}

    virtual void putSpace(const std::string &) {}

    virtual void putKeyword(const std::string &) {}

    virtual void putError(const std::string &) {}

    virtual void putMainBegin() {}

    virtual void putMainEnd() {}

    virtual void putPlaceHolder() {}

    virtual void putBegin() {}

    virtual void putEnd() {}

    virtual void putLnBegin() {}

    virtual void putLnEnd() {}

    virtual void putLn(const bool) {}

public:
    inline Pass(
        std::ostream &target,
        const bool verbose, const bool compact
    ):
        PassProto<PASS_REPR>(),
        optionV(verbose), optionC(compact),
        out(target), indent(0) {}

    // virtual ~Pass() {}

    // specialized nodes

    template <size_t I>
    void run(const NodeSpace<I> *node) {
        putMainBegin();

        putName(node->getRuleName());

        putBegin();
        putSpace(node->getFullText());
        putEnd();

        putMainEnd();
    }

    template <size_t I>
    void run(const NodeKeyword<I> *node) {
        putMainBegin();

        putName(node->getRuleName());

        putBegin();
        putKeyword(node->getFullText());
        putEnd();

        putMainEnd();
    }

    // common nodes

    template <size_t I>
    void run(const NodeListIndexed<I> *node) {
        putMainBegin();

        putName(node->getRuleName());
        putIndex(I); // getIndex() == I

        std::vector<Node<> *> children1;

        if (!optionV) {
            for (Node<> *child: node->getChildren()) {
                if (!child->empty()) {
                    children1.push_back(child);
                }
            }
        }

        const std::vector<Node<> *> &children =
            optionV ? node->getChildren() : children1;

        if (optionC && children.size() == 1) {
            putBegin();
            children[0]->runPass(this);
            putEnd();
        } else {
            putPlaceHolder();

            putLnBegin();
            ++indent;

            bool first = true;
            for (const Node<> *child: children) {
                putLn(first);
                first = false;
                child->runPass(this);
            }

            --indent;
            putLnEnd();
        }

        putMainEnd();
    }

    template <class TX = void> // actually not a template
    void run(const NodeTextPure<> *node) {
        putMainBegin();

        putName(node->getRuleName());

        putBegin();
        putText(node->getFullText());
        putEnd();

        putMainEnd();
    }

    template <class E>
    void run(const NodeTextOrError<E> *node) {
        putMainBegin();

        putName(node->getRuleName());

        putBegin();
        if (node->accepted()) {
            putText(node->getFullText());
        } else {
            putError(E::getStr());
        }
        putEnd();

        putMainEnd();
    }

    template <class E>
    void run(const NodeError<E> *node) {
        putMainBegin();

        putName(node->getRuleName());

        putBegin();
        putError(E::getStr());
        putEnd();

        putMainEnd();
    }
};

template <class TX = void> // actually not a template
class PassReprText: public Pass<PASS_REPR> {
protected:
    virtual void putText(const std::string &text) {
        out << text;
    }

    virtual void putSpace(const std::string &text) {
        out << text;
    }

    virtual void putKeyword(const std::string &text) {
        out << text;
    }

public:
    inline PassReprText(
        std::ostream &target
    ): Pass<PASS_REPR>(target, true, false) {}

    // virtual ~PassReprText() {}
};

template <class TX = void> // actually not a template
class PassReprSimple: public Pass<PASS_REPR> {
protected:
    virtual void putText(const std::string &text) {
        out << style_string << style_underline << text << style_normal;
    }

    virtual void putSpace(const std::string &text) {
        out << style_space << style_underline << text << style_normal;
    }

    virtual void putKeyword(const std::string &text) {
        out << style_keyword << style_underline << text << style_normal;
    }

    virtual void putError(const std::string &error) {
        out << style_error << "ERROR: " << style_normal << error;
    }

    virtual void putPlaceHolder() {
        out << "=>";
    }

    virtual void putLn(const bool) {
        out << '\n';
        for (size_t i = 0; i < indent; ++i) {
            out << "    ";
        }
    }

public:
    inline PassReprSimple(
        std::ostream &target,
        const bool verbose = false, const bool compact = true
    ): Pass<PASS_REPR>(target, verbose, compact) {}

    // virtual ~PassReprSimple() {}
};

template <class TX = void> // actually not a template
class PassReprFull: public Pass<PASS_REPR> {
protected:
    virtual void putName(const std::string &name) {
        out << name;
    }

    virtual void putIndex(const size_t index) {
        out << '[';
        out << style_data << index << style_normal;
        out << ']';
    }

    virtual void putText(const std::string &text) {
        out << style_string << style_underline << text << style_normal;
    }

    virtual void putSpace(const std::string &text) {
        out << style_space << style_underline << text << style_normal;
    }

    virtual void putKeyword(const std::string &text) {
        out << style_keyword << style_underline << text << style_normal;
    }

    virtual void putError(const std::string &error) {
        out << style_error << "ERROR: " << style_normal << error;
    }

    virtual void putBegin() {
        out << style_space << " - " << style_normal;
    }

    virtual void putLn(const bool) {
        out << '\n';
        for (size_t i = 0; i < indent; ++i) {
            out << "    ";
        }
    }

public:
    inline PassReprFull(
        std::ostream &target,
        const bool verbose = false, const bool compact = true
    ): Pass<PASS_REPR>(target, verbose, compact) {}

    // virtual ~PassReprFull() {}
};

template <class TX = void> // actually not a template
class PassReprJSON: public Pass<PASS_REPR> {
protected:
    void putStrEscaped(const std::string &str) {
        for (const char c: str) {
            switch (c) {
            // case '\0':
                // never reach
            case '\b':
                out << "\\b";
                break;
            case '\t':
                out << "\\t";
                break;
            case '\n':
                out << "\\n";
                break;
            case '\v':
                out << "\\v";
                break;
            case '\f':
                out << "\\f";
                break;
            case '\r':
                out << "\\r";
                break;
            case '\"':
                out << "\\\"";
                break;
            case '\'':
                out << "\\\'";
                break;
            case '\\':
                out << "\\\\";
                break;
            case '\x7F':
                out << "\\x7F";
                break;
            default:
                if ('\0' <= c && c < '\x10') {
                    out << "\\x0" << ("0123456789ABCDEF"[c & 0xF]);
                } else if ('\x10' <= c && c < '\x20') {
                    out << "\\x1" << ("0123456789ABCDEF"[c & 0xF]);
                } else {
                    out << c;
                }
                break;
            }
        }
    }

    virtual void putName(const std::string &name) {
        putLn1();
        out << "\"rulename\": \"" << name << "\",";
    }

    virtual void putIndex(const size_t index) {
        putLn1();
        out << "\"ruleindex\": \"" << index << "\",";
    }

    virtual void putText(const std::string &text) {
        putLn1();
        out << "\"text\": \"";
        putStrEscaped(text);
        out << "\",";
    }

    virtual void putSpace(const std::string &text) {
        putLn1();
        out << "\"text\": \"";
        putStrEscaped(text);
        out << "\",";
    }

    virtual void putKeyword(const std::string &text) {
        putLn1();
        out << "\"text\": \"";
        putStrEscaped(text);
        out << "\",";
    }

    virtual void putError(const std::string &error) {
        putLn1();
        out << "\"error\": \"";
        putStrEscaped(error);
        out << "\",";
    }

    virtual void putMainBegin() {
        out << "{";
        ++indent; // extra
    }

    virtual void putMainEnd() {
        --indent; // extra
        putLn1();
        out << "}";
    }

    virtual void putLnBegin() {
        putLn1();
        out << "\"children\": [";
        --indent; // extra
    }

    virtual void putLnEnd() {
        ++indent; // extra
        out << "],";
    }

    virtual void putLn(const bool first) {
        if (!first) {
            out << ',';
            putLn1();
        }
    }

    void putLn1() {
        out << '\n';
        for (size_t i = 0; i < indent; ++i) {
            out << "    ";
        }
    }

public:
    inline PassReprJSON(
        std::ostream &target, const bool verbose = false
    ): Pass<PASS_REPR>(target, verbose, false) {}

    // virtual ~PassReprJSON() {}
};

}
