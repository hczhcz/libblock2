#pragma once

#include "myparser_ast.hpp"

namespace myparser {

template <size_t I>
class NodeSpace: public NodeListIndexed<I> {
public:
    inline NodeSpace(
        const Input &input
    ): NodeListIndexed<I>(input) {}

    // virtual ~NodeSpace() {}

    virtual bool empty() const {
        return true;
    }
};

template <size_t I>
class NodeKeyword: public NodeListIndexed<I> {
public:
    inline NodeKeyword(
        const Input &input
    ): NodeListIndexed<I>(input) {}

    // virtual ~NodeKeyword() {}
};

template <class T, class E>
class NodeData: public NodeTextOrError<E> {
private:
    T data;
    bool succeed;

public:
    inline NodeData(
        const Input &input, const Input &end
    ): NodeTextOrError<E>(input, end) {
        // TODO: optimize?
        std::istringstream conv(std::string(input, end));

        succeed = (conv >> data) && conv.eof();
    }

    // virtual ~NodeData() {}

    virtual bool accepted() const {
        return succeed;
    }

    inline const T &getData() const {
        return data;
    }
};

template <size_t QL, size_t QR, char EC, class E>
class NodeString: public NodeTextOrError<E> {
private:
    std::string raw;
    bool succeed;

    inline char hexMap(const char value) const {
        if ('0' <= value && value <= '9') {
            return value - '0';
        } else if ('A' <= value && value <= 'F') {
            return value - 'A' + 10;
        } else if ('a' <= value && value <= 'f') {
            return value - 'a' + 10;
        } else {
            // error
            return -1;
        }
    }

    inline char hexMap2(const char value1, const char value2) const {
        char hi = hexMap(value1);
        char lo = hexMap(value2);

        if (hi < 0 || lo < 0) {
            return -1;
        } else {
            return (hi << 3) ^ lo;
        }
    }

public:
    inline NodeString(
        const Input &input, const Input &end
    ): NodeTextOrError<E>(input, end) {
        std::ostringstream result;

        for (Input i = input + QL; i < end - QR; ++i) {
            if (*i == EC) {
                // escape

                ++i;
                if (i >= end - QR) {
                    succeed = false;
                    return;
                }

                switch (*i) {
                case '0':
                    result << '\0';
                    break;
                case 'b':
                    result << "\b";
                    break;
                case 't':
                    result << "\t";
                    break;
                case 'n':
                    result << "\n";
                    break;
                case 'v':
                    result << "\v";
                    break;
                case 'f':
                    result << "\f";
                    break;
                case 'r':
                    result << "\r";
                    break;
                case '\"':
                    result << "\"";
                    break;
                case '\'':
                    result << "\'";
                    break;
                case '\\':
                    result << "\\";
                    break;
                case 'x':
                    // "\x00"

                    i += 2;
                    if (i >= end - QR) {
                        succeed = false;
                        return;
                    }

                    {
                        char hvalue = hexMap2(*(i - 1), *i);
                        if (hvalue < 0) {
                            succeed = false;
                            return;
                        }

                        result << hvalue;
                    }

                    break;
                default:
                    result << *i;
                    break;
                }
            } else {
                // no escape

                result << *i;
            }
        }

        succeed = true;
        raw = result.str();
    }

    // virtual ~NodeString() {}

    virtual bool accepted() const {
        return succeed;
    }

    inline const std::string &getRaw() const {
        return raw;
    }
};

// specialization

template <>
class NodeBaseList<BuiltinSpace> {
public:
    template <size_t I>
    using Type = NodeSpace<I>;
};

template <>
class NodeBaseList<BuiltinKeyword> {
public:
    template <size_t I>
    using Type = NodeKeyword<I>;
};

}
