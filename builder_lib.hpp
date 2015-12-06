#pragma once

#include "builder_type.hpp"
#include "builder_block.hpp"

namespace libblock {

namespace builder {

template <class T>
std::function<Block *()> libFuncT() {
    return []() {
        return new T {};
    };
}

inline std::function<Block *()> libFunc2R(std::string &&str) {
    return [str = std::move(str)]() mutable {
        return new BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a")},
            {typeInt("a"), typeInt("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block *()> libFunc2A(std::string &&str) {
    return [str = std::move(str)]() mutable {
        return new BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a")},
            {typeInt("input"), typeInt("a")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block *()> libFunc3R(std::string &&str) {
    return [str = std::move(str)]() mutable {
        return new BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a"), in("b")},
            {typeInt("a"), typeInt("b"), typeInt("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block *()> libFunc3A(std::string &&str) {
    return [str = std::move(str)]() mutable {
        return new BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a"), in("b")},
            {typeInt("input"), typeInt("a"), typeInt("b")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block *()> libFunc3B(std::string &&str) {
    return [str = std::move(str)]() mutable {
        return new BlockBuiltinFmt {
            {BlockOption::allow_in},
            {in("a"), out("b")},
            {typeInt("input"), typeInt("a"), typeInt("b")},
            {},
            std::move(str)
        };
    };
}

}

}
