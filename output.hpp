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

    void enter();
    void leave();
    void endl();
};

class OutputTask {
private:
    std::list<std::function<void (OutputContext &)>> render_funcs;

public:
    void insert(std::function<void (OutputContext &)> &&render);
    void generate(OutputContext &oc);
};

class Output {
private:
    std::map<
        std::reference_wrapper<Instance>,
        std::unique_ptr<OutputTask>
    > headers;
    std::map<
        std::reference_wrapper<Instance>,
        std::unique_ptr<OutputTask>
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
