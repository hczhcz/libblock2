#pragma once

#include "builder_type.hpp"
#include "builder_block.hpp"

namespace libblock {

namespace builder {

template <class T>
std::function<Block &()> libFuncT() {
    return []() -> Block & {
        return *new (GC) T {};
    };
}

inline std::function<Block &()> libFuncB2R(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a")},
            {typeBool("a"), typeBool("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncB2A(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a")},
            {typeBool("input"), typeBool("a")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncB3R(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a"), in("b")},
            {typeBool("a"), typeBool("b"), typeBool("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncB3A(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a"), in("b")},
            {typeBool("input"), typeBool("a"), typeBool("b")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncB3B(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {in("a"), out("b")},
            {typeBool("input"), typeBool("a"), typeBool("b")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncI2R(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a")},
            {typeInt("a"), typeInt("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncI2A(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a")},
            {typeInt("input"), typeInt("a")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncI3R(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a"), in("b")},
            {typeInt("a"), typeInt("b"), typeInt("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncI3A(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a"), in("b")},
            {typeInt("input"), typeInt("a"), typeInt("b")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncI3B(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {in("a"), out("b")},
            {typeInt("input"), typeInt("a"), typeInt("b")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncR2R(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a")},
            {typeReal("a"), typeReal("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncR2A(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a")},
            {typeReal("input"), typeReal("a")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncR3R(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a"), in("b")},
            {typeReal("a"), typeReal("b"), typeReal("result")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncR3A(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a"), in("b")},
            {typeReal("input"), typeReal("a"), typeReal("b")},
            {},
            std::move(str)
        };
    };
}

inline std::function<Block &()> libFuncR3B(std::string &&str) {
    return [str = std::move(str)]() mutable -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_in},
            {in("a"), out("b")},
            {typeReal("input"), typeReal("a"), typeReal("b")},
            {},
            std::move(str)
        };
    };
}

}

}
