#pragma once

#include <string>

namespace util {

std::string cCode(bool &value);
std::string cCode(int64_t &value);
std::string cCode(double &value);
std::string cCode(std::string &value);
std::string cCode(uintptr_t &value);

std::string cType(std::string &&name, bool &value);
std::string cType(std::string &&name, int64_t &value);
std::string cType(std::string &&name, double &value);
std::string cType(std::string &&name, std::string &value);
std::string cType(std::string &&name, uintptr_t &value);

}
