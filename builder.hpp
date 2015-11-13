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
    return new NodeSymbol {std::move(name), LookupMode::mixed};
}

inline NodeSymbol *$(std::string &&name, LookupMode mode) {
    return new NodeSymbol {std::move(name), mode};
}

inline NodePath *$(Node *source, std::string &&name) {
    return new NodePath {source, LookupMode::local, std::move(name)};
}

inline NodePath *$(Node *source, std::string &&name, LookupMode mode) {
    return new NodePath {source, mode, std::move(name)};
}

template <class... Args>
inline NodeCall *call(Node *source, Args... args) {
    return new NodeCall {source, FrameMode::dynamic_gc, args...};
}

template <class... Args>
inline NodeCall *call(Node *source, FrameMode mode, Args... args) {
    return new NodeCall {source, mode, args...};
}

template <class... Blocks>
inline NodeBlock *blocks(Blocks... blocks) {
    return new NodeBlock {blocks...};
}

inline BlockUser *block(
    std::vector<std::pair<std::string, ParamMode>> &&params,
    Node *ast
) {
    return new BlockUser {std::move(params), ast};
}

inline std::pair<std::string, ParamMode> in(
    std::string &&name
) {
    return {std::move(name), ParamMode::in};
}

inline std::pair<std::string, ParamMode> out(
    std::string &&name
) {
    return {std::move(name), ParamMode::out};
}

inline std::pair<std::string, ParamMode> var(
    std::string &&name
) {
    return {std::move(name), ParamMode::var};
}

inline std::pair<std::string, ParamMode> special(
    std::string &&name
) {
    return {std::move(name), ParamMode::special};
}

}
