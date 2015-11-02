#pragma once

#include <string>

namespace util {

std::string cCode(bool &value);
std::string cCode(int64_t &value);
std::string cCode(double &value);
std::string cCode(std::string &value);
std::string cCode(uintptr_t &value);

}
