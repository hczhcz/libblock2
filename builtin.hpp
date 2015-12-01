#pragma once

#include "include.hpp"

namespace libblock {

class Instance;
class NodeBlock;
class Block;

class BuiltinContainer {
private:
    std::map<std::string, std::unique_ptr<NodeBlock>> nodes;

public:
    void apply(Instance &instance);
};

class Builtin {
private:
    static std::list<std::reference_wrapper<Builtin>> &all();

    std::string name;
    std::vector<std::function<Block *()>> funcs;

    friend class BuiltinContainer;

public:
    Builtin(
        std::string &&_name,
        std::vector<std::function<Block *()>> &&_funcs
    );
};

}
