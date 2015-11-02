#pragma once

#include "node.hpp"

namespace builder {

inline NodeLiteralBool *_(bool &&value) {
    return new NodeLiteralBool {std::move(value)};
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
    return new NodeSymbol {{std::move(name)}};
}

inline NodeSymbol *$(std::vector<std::string> &&path) {
    return new NodeSymbol {std::move(path)};
}

template <class... Args>
inline NodeCall *call(Node *callee, Args... args) {
    return new NodeCall {callee, args...};
}

inline Block *block(
    std::vector<std::pair<std::string, SymbolMode>> &&params,
    Node *ast
) {
    return new Block {std::move(params), ast};
}

inline std::pair<std::string, SymbolMode> in(std::string &&name) {
    return {std::move(name), SymbolMode::in};
}

inline std::pair<std::string, SymbolMode> out(std::string &&name) {
    return {std::move(name), SymbolMode::out};
}

inline std::pair<std::string, SymbolMode> var(std::string &&name) {
    return {std::move(name), SymbolMode::var};
}

}
