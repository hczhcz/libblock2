#pragma once

#include "type.hpp"

namespace builder {

inline std::pair<std::string, Type &> typeVoid(
    std::string &&name
) {
    return {std::move(name), TypeNativeVoid::get()};
}

inline std::pair<std::string, Type &> typeBool(
    std::string &&name
) {
    return {std::move(name), TypeNativeBool::get()};
}

inline std::pair<std::string, Type &> typeInt(
    std::string &&name
) {
    return {std::move(name), TypeNativeInt::get()};
}

inline std::pair<std::string, Type &> typeReal(
    std::string &&name
) {
    return {std::move(name), TypeNativeReal::get()};
}

inline std::pair<std::string, Type &> typeStr(
    std::string &&name
) {
    return {std::move(name), TypeNativeStr::get()};
}

}
