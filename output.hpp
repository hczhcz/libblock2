#pragma once

#include <map>
#include <sstream>

struct OutputContext {
    std::ostringstream header;
    std::ostringstream content;
};

struct Output: public std::map<uintptr_t, OutputContext> {
    //
};
