#pragma once

#include "include.hpp"

namespace libblock {

class Instance;

class Session {
private:
    std::gc_list<
        std::reference_wrapper<Instance>
    > instances;

public:
    void insert(Instance &instance);
    void each(std::gc_function<void (Instance &)> &&func);

    // TODO: use export instead of root
    void renderHeader(std::ostream &os, Instance &root) const;
    void renderContent(std::ostream &os, Instance &root) const;
};

}
