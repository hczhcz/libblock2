#pragma once

#ifndef NO_STD_LIB
#include <map>
#include <sstream>
#endif

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
