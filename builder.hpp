#include "block.hpp"

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
    std::vector<std::string> &&params,
    std::map<std::string, SymbolInfo> &&symbols,
    Node *ast
) {
    return new Block {std::move(params), std::move(symbols), ast};
}

using symbol_pair_t = std::pair<std::string, SymbolInfo>;

inline symbol_pair_t var(std::string &&name) {
    return {std::move(name), {true, true}};
}

inline symbol_pair_t in(std::string &&name) {
    return {std::move(name), {true, false}};
}

inline symbol_pair_t out(std::string &&name) {
    return {std::move(name), {false, true}};
}

inline symbol_pair_t tmp(std::string &&name) {
    return {std::move(name), {false, false}};
}

}
