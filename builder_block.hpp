#pragma once

#include "block.hpp"

namespace libblock {

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

inline BlockUser &block(
    std::gc_vector<std::pair<std::string, ParamMode>> &&params,
    Node &ast
) { // default
    return *new (GC) BlockUser {
        {
            BlockOption::allow_proc,
            BlockOption::allow_out,
            BlockOption::allow_in
        },
        std::move(params),
        ast
    };
}

inline BlockUser &block(
    std::set<BlockOption> &&options,
    std::gc_vector<std::pair<std::string, ParamMode>> &&params,
    Node &ast
) {
    return *new (GC) BlockUser {
        std::move(options),
        std::move(params),
        ast
    };
}

}

}
