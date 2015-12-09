#pragma once

#include "include.hpp"

namespace libblock {

class Instance;
class NodeBlock;
class Block;

class BuiltinContainer {
private:
    std::gc_map<
        std::string,
        std::reference_wrapper<NodeBlock>
    > nodes;

public:
    void apply(
        std::string &&package,
        Instance &instance
    );
};

class Builtin {
private:
    static std::gc_map<
        std::string,
        std::gc_list<std::reference_wrapper<Builtin>>
    > &all();

    std::string name;
    std::gc_vector<std::function<Block &()>> funcs;

    friend class BuiltinContainer;

public:
    Builtin(
        std::string &&package,
        std::string &&_name,
        std::gc_vector<std::function<Block &()>> &&_funcs
    );
};

}
