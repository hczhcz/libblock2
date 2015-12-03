#pragma once

#include "myparser_common.hpp"

#define MP_STR(str) myparser::StaticStr \
            <myparser::strIndex((str), 0)>::Append<myparser::strIndex((str), 1)> \
    ::Append<myparser::strIndex((str), 2)>::Append<myparser::strIndex((str), 3)> \
    ::Append<myparser::strIndex((str), 4)>::Append<myparser::strIndex((str), 5)> \
    ::Append<myparser::strIndex((str), 6)>::Append<myparser::strIndex((str), 7)> \
    ::Append<myparser::strIndex((str), 8)>::Append<myparser::strIndex((str), 9)> \
    ::Append<myparser::strIndex((str), 10)>::Append<myparser::strIndex((str), 11)> \
    ::Append<myparser::strIndex((str), 12)>::Append<myparser::strIndex((str), 13)> \
    ::Append<myparser::strIndex((str), 14)>::Append<myparser::strIndex((str), 15)> \
    ::Append<myparser::strIndex((str), 16)>::Append<myparser::strIndex((str), 17)> \
    ::Append<myparser::strIndex((str), 18)>::Append<myparser::strIndex((str), 19)> \
    ::Append<myparser::strIndex((str), 20)>::Append<myparser::strIndex((str), 21)> \
    ::Append<myparser::strIndex((str), 22)>::Append<myparser::strIndex((str), 23)> \
    ::Append<myparser::strIndex((str), 24)>::Append<myparser::strIndex((str), 25)> \
    ::Append<myparser::strIndex((str), 26)>::Append<myparser::strIndex((str), 27)> \
    ::Append<myparser::strIndex((str), 28)>::Append<myparser::strIndex((str), 29)> \
    ::Append<myparser::strIndex((str), 30)>::Append<myparser::strIndex((str), 31)> \
    ::Append<myparser::strIndex((str), 32)>::Append<myparser::strIndex((str), 33)> \
    ::Append<myparser::strIndex((str), 34)>::Append<myparser::strIndex((str), 35)> \
    ::Append<myparser::strIndex((str), 36)>::Append<myparser::strIndex((str), 37)> \
    ::Append<myparser::strIndex((str), 38)>::Append<myparser::strIndex((str), 39)> \
    ::Append<myparser::strIndex((str), 40)>::Append<myparser::strIndex((str), 41)> \
    ::Append<myparser::strIndex((str), 42)>::Append<myparser::strIndex((str), 43)> \
    ::Append<myparser::strIndex((str), 44)>::Append<myparser::strIndex((str), 45)> \
    ::Append<myparser::strIndex((str), 46)>::Append<myparser::strIndex((str), 47)> \
    ::Append<myparser::strIndex((str), 48)>::Append<myparser::strIndex((str), 49)> \
    ::Append<myparser::strIndex((str), 50)>::Append<myparser::strIndex((str), 51)> \
    ::Append<myparser::strIndex((str), 52)>::Append<myparser::strIndex((str), 53)> \
    ::Append<myparser::strIndex((str), 54)>::Append<myparser::strIndex((str), 55)> \
    ::Append<myparser::strIndex((str), 56)>::Append<myparser::strIndex((str), 57)> \
    ::Append<myparser::strIndex((str), 58)>::Append<myparser::strIndex((str), 59)> \
    ::Append<myparser::strIndex((str), 60)>::Append<myparser::strIndex((str), 61)> \
    ::Append<myparser::strIndex((str), 62)>::Append<myparser::strIndex((str), 63)>

namespace myparser {

constexpr size_t strLen(const char *str, size_t index = 0) {
    return str[index] == 0 ? index : strLen(str, index + 1);
}

constexpr char strIndex(const char *str, size_t index) {
    return index < strLen(str) ? str[index] : 0;
}

template <char TAIL, char... Cx>
class StaticStr {
private:
    inline StaticStr() = delete; // force static

    // virtual ~StaticStr() = delete;

public:
    template <char C>
    using Append = StaticStr<C, Cx..., TAIL>;
};

template <char... Cx>
class StaticStr<0, Cx...> {
private:
    inline StaticStr() = delete; // force static

    // virtual ~StaticStr() = delete;

public:
    static inline const std::string &getStr() {
        static const std::string stdstr = {Cx...};

        return stdstr;
    }

    template <char C>
    using Append = StaticStr<0, Cx...>;
};

}
