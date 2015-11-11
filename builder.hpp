#pragma once

#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace builder {

inline NodeLiteralBool *_true() {
    return new NodeLiteralBool {true};
}

inline NodeLiteralBool *_false() {
    return new NodeLiteralBool {false};
}

inline NodeLiteralInt *_(int32_t &&value) {
    return new NodeLiteralInt {std::move(value)};
}

inline NodeLiteralInt *_(int64_t &&value) {
    return new NodeLiteralInt {std::move(value)};
}

inline NodeLiteralReal *_(double &&value) {
    return new NodeLiteralReal {std::move(value)};
}

inline NodeLiteralStr *_(std::string &&value) {
    return new NodeLiteralStr {std::move(value)};
}

inline NodeSymbol *$(std::string &&name) {
    return new NodeSymbol {std::move(name)};
}

inline NodePath *$(Node *source, std::string &&name) {
    return new NodePath {source, std::move(name)};
}

template <class... Args>
inline NodeCall *call(Node *source, Args... args) {
    return new NodeCall {source, CallMode::dynamic_gc, args...};
}

template <class... Args>
inline NodeCall *call(Node *source, CallMode mode, Args... args) {
    return new NodeCall {source, mode, args...};
}

template <class... Blocks>
inline NodeBlock *blocks(Blocks... blocks) {
    return new NodeBlock {blocks...};
}

inline BlockUser *block(
    std::vector<std::pair<std::string, SymbolMode>> &&params,
    Node *ast
) {
    return new BlockUser {std::move(params), ast};
}

inline std::pair<std::string, SymbolMode> in(
    std::string &&name
) {
    return {std::move(name), SymbolMode::in};
}

inline std::pair<std::string, SymbolMode> out(
    std::string &&name
) {
    return {std::move(name), SymbolMode::out};
}

inline std::pair<std::string, SymbolMode> var(
    std::string &&name
) {
    return {std::move(name), SymbolMode::var};
}

inline std::pair<std::string, SymbolMode> special(
    std::string &&name
) {
    return {std::move(name), SymbolMode::special};
}

}
