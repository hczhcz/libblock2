#pragma once

#include "node.hpp"

namespace libblock {

namespace builder {

inline NodeLiteralVoid &void_lit() {
    return *new (GC) NodeLiteralVoid {nullptr};
}

inline NodeLiteralBool &true_lit() {
    return *new (GC) NodeLiteralBool {true};
}

inline NodeLiteralBool &false_lit() {
    return *new (GC) NodeLiteralBool {false};
}

inline NodeLiteralInt &operator ""_lit(unsigned long long value) {
    return *new (GC) NodeLiteralInt {value};
}

inline NodeLiteralReal &operator ""_lit(long double value) {
    return *new (GC) NodeLiteralReal {value};
}

inline NodeLiteralStr &operator ""_lit(const char *value, size_t size) {
    return *new (GC) NodeLiteralStr {{value, size}};
}

inline NodeSymbol &$(std::string &&name) { // default
    return *new (GC) NodeSymbol {
        LookupMode::mixed, std::move(name)
    };
}

inline NodeSymbol &$(LookupMode mode, std::string &&name) {
    return *new (GC) NodeSymbol {
        mode, std::move(name)
    };
}

inline NodePath &$(Node &source, std::string &&name) { // default
    return *new (GC) NodePath {
        source, LookupMode::local, std::move(name)
    };
}

inline NodePath &$(Node &source, LookupMode mode, std::string &&name) {
    return *new (GC) NodePath {
        source, mode, std::move(name)
    };
}

template <class... Args>
inline NodeCall &call(Node &source, Args... args) {
    return *new (GC) NodeCall {
        source, args...
    };
}

template <class... Blocks>
inline NodeBlock &blocks(Blocks... blocks) { // default
    return *new (GC) NodeBlock {
        *new (GC) NodeSymbol {LookupMode::local, "self"}, blocks...
    };
}

template <class... Blocks>
inline NodeBlock &blocks(Node &source, Blocks... blocks) {
    return *new (GC) NodeBlock {
        source, blocks...
    };
}

}

}
