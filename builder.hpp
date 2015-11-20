#pragma once

#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace builder {

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

inline NodeSymbol *$(std::string &&name) {
    return new NodeSymbol {LookupMode::mixed, std::move(name)};
}

inline NodeSymbol *$(LookupMode mode, std::string &&name) {
    return new NodeSymbol {mode, std::move(name)};
}

inline NodePath *$(Node *source, std::string &&name) {
    return new NodePath {source, LookupMode::local, std::move(name)};
}

inline NodePath *$(Node *source, LookupMode mode, std::string &&name) {
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
    return new BlockUser {
        {
            BlockOption::parent,
            BlockOption::allow_proc,
            BlockOption::allow_out
        },
        std::move(params),
        ast
    };
}

inline BlockUser *block(
    std::set<BlockOption> &&options,
    std::vector<std::pair<std::string, ParamMode>> &&params,
    Node *ast
) {
    return new BlockUser {
        std::move(options),
        std::move(params),
        ast
    };
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
