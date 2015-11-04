#pragma once

#include <map>
#include <sstream>

struct Instance;

struct OutputContext {
    std::ostringstream header;
    std::ostringstream content;
};

struct Output {
    std::map<uintptr_t, OutputContext &> map;

    virtual ~Output();

    OutputContext &at(Instance &instance);
    OutputContext &insert(Instance &instance);
};
