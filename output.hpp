#pragma once

#include "include.hpp"

namespace libblock {

class Instance;

class OutputContext {
private:
    size_t indent {0};

public:
    std::ostream &os;

    OutputContext(std::ostream &_os);

    void tab(); // print indention
    void enter();
    void leave();
    void endl();
};

class OutputTask {
private:
    std::gc_list<std::gc_function<void (OutputContext &)>> render_funcs;

public:
    void insert(std::gc_function<void (OutputContext &)> &&render);
    void generate(OutputContext &oc);
};

}
