#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace util {

std::string cCode(bool &value) {
    return value ? "true" : "false";
}

std::string cCode(int64_t &value) {
    return std::to_string(value);
}

std::string cCode(double &value) {
    // return std::to_string(value);
    return "*(double *) (uint64_t []) {"
        + std::to_string(*(uint64_t *) &value) + "}";
}

std::string cCode(std::string &value) {
    std::string result;

    result += "(const char []) {";
    for (char i: value) {
        result += std::to_string((int) i) + ",";
    }
    result += "}";

    return result;
}

std::string cCode(uintptr_t &value) {
    return std::to_string(value);
}

std::string cType(std::string &&name, bool &value) {
    return "bool " + name;
}

std::string cType(std::string &&name, int64_t &value) {
    return "int64_t " + name;
}

std::string cType(std::string &&name, double &value) {
    return "double " + name;
}

std::string cType(std::string &&name, std::string &value) {
    return "const char " + name + "[]";
}

std::string cType(std::string &&name, uintptr_t &value) {
    return "uintptr_t " + name;
}

}
