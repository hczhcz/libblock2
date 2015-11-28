#pragma once

#include "block.hpp"

namespace builder {

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

inline BlockUser *block(
    std::vector<std::pair<std::string, ParamMode>> &&params,
    Node *ast
) { // default
    return new BlockUser {
        {
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

}
