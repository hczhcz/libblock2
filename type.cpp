#include "block.hpp"

template <>
std::string TypeNative<bool>::typeName() {
    return "bool";
}

template <>
std::string TypeNative<int64_t>::typeName() {
    return "int";
}

template <>
std::string TypeNative<double>::typeName() {
    return "real";
}

template <>
std::string TypeNative<std::string>::typeName() {
    return "str";
}

std::string Block::typeName() {
    return "block" + std::to_string(tuid());
}

std::string Instance::typeName() {
    return "instance_" + std::to_string(tuid());
}
