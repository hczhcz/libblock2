#pragma once

#include "include.hpp"

class Instance;

class OutputContext {
private:
    size_t indent {0};

public:
    std::ostringstream os;

    void enter();
    void leave();
    void endl();
};

class Output {
private:
    std::map<uintptr_t, std::shared_ptr<OutputContext>> headers;
    std::map<uintptr_t, std::shared_ptr<OutputContext>> contents;

public:
    OutputContext &header(Instance &instance);
    OutputContext &content(Instance &instance);
    void insert(Instance &instance);

    void getHeader(std::ostream &os, Instance &root) const;
    void getContent(std::ostream &os, Instance &root) const;
};
