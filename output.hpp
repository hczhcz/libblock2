#pragma once

#ifndef NO_STD_LIB
#include <memory>
#include <vector>
#include <map>
#include <sstream>
#endif

struct Instance;

struct OutputContext {
    std::ostringstream header;
    std::ostringstream content;
};

struct Output {
    std::vector<std::unique_ptr<OutputContext>> members;
    std::map<uintptr_t, OutputContext &> map;

    OutputContext &at(Instance &instance);
    OutputContext &insert(Instance &instance);

    void getHeader(std::ostream &os) const;
    void getContent(std::ostream &os) const;
};
