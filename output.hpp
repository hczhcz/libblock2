#pragma once

#ifndef NO_STD_LIB
#include <memory>
#include <vector>
#include <map>
#include <sstream>
#endif

class Instance;

class OutputContext {
private:
    size_t indent;
    // std::ostringstream os; // TODO

    friend class Output;

public:
    std::ostringstream os; // TODO

    void endl(size_t offset);
};

class Output {
private:
    std::map<uintptr_t, std::shared_ptr<OutputContext>> headers;
    std::map<uintptr_t, std::shared_ptr<OutputContext>> contents;

public:
    OutputContext &header(Instance &instance);
    OutputContext &content(Instance &instance);
    void insert(Instance &instance);

    void getHeader(std::ostream &os) const;
    void getContent(std::ostream &os) const;
};
