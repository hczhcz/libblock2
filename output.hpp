#pragma once

#include <map>
#include <sstream>

struct OutputContext {
    std::ostringstream header;
    std::ostringstream content;
};

struct Output {
    std::map<uintptr_t, OutputContext &> map;

    ~Output();

    OutputContext &at(uintptr_t key);
    OutputContext &insert(uintptr_t key);
};
