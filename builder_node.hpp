#pragma once

#include "node.hpp"

namespace builder {

inline NodeLiteralVoid *void_lit() {
    return new NodeLiteralVoid {nullptr};
}

inline NodeLiteralBool *true_lit() {
    return new NodeLiteralBool {true};
}

inline NodeLiteralBool *false_lit() {
    return new NodeLiteralBool {false};
}

inline NodeLiteralInt *operator ""_lit(unsigned long long value) {
    return new NodeLiteralInt {value};
}

inline NodeLiteralReal *operator ""_lit(long double value) {
    return new NodeLiteralReal {value};
}

inline NodeLiteralStr *operator ""_lit(const char *value, size_t size) {
    return new NodeLiteralStr {{value, size}};
}

inline NodeSymbol *$(std::string &&name) { // default
    return new NodeSymbol {
        LookupMode::mixed, std::move(name)
    };
}

inline NodeSymbol *$(LookupMode mode, std::string &&name) {
    return new NodeSymbol {
        mode, std::move(name)
    };
}

inline NodePath *$(Node *source, std::string &&name) { // default
    return new NodePath {
        source, LookupMode::local, std::move(name)
    };
}

inline NodePath *$(Node *source, LookupMode mode, std::string &&name) {
    return new NodePath {
        source, mode, std::move(name)
    };
}

template <class... Args>
inline NodeCall *call(Node *source, Args... args) { // default
    return new NodeCall {
        source, FrameMode::dynamic_gc, args...
    };
}

template <class... Args>
inline NodeCall *call(Node *source, FrameMode mode, Args... args) {
    return new NodeCall {
        source, mode, args...
    };
}

template <class... Blocks>
inline NodeBlock *blocks(Blocks... blocks) { // default
    return new NodeBlock {
        new NodeSymbol {LookupMode::local, "self"}, blocks...
    };
}

template <class... Blocks>
inline NodeBlock *blocks(Node *source, Blocks... blocks) {
    return new NodeBlock {
        source, blocks...
    };
}

}
