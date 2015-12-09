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
    std::gc_list<std::function<void (OutputContext &)>> render_funcs;

public:
    void insert(std::function<void (OutputContext &)> &&render);
    void generate(OutputContext &oc);
};

class Output {
private:
    std::gc_map<
        std::reference_wrapper<Instance>,
        std::reference_wrapper<OutputTask>
    > headers;
    std::gc_map<
        std::reference_wrapper<Instance>,
        std::reference_wrapper<OutputTask>
    > contents;

public:
    void header(
        Instance &instance,
        std::function<void (OutputContext &)> &&render
    );
    void content(
        Instance &instance,
        std::function<void (OutputContext &)> &&render
    );

    void insert(Instance &instance);

    void getHeader(std::ostream &os, Instance &root) const;
    void getContent(std::ostream &os, Instance &root) const;
};

}
