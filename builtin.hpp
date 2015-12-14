#pragma once

#include "include.hpp"

namespace libblock {

class Instance;
class NodeBlock;
class Block;

class Builtin {
private:
    static std::gc_map<
        std::string,
        std::gc_list<std::reference_wrapper<Builtin>>
    > &all();

    std::string name;
    std::gc_vector<std::gc_function<Block &()>> funcs;

    friend class BuiltinContainer;

public:
    static void apply(
        std::string &&package,
        Instance &instance
    );

    Builtin(
        std::string &&package,
        std::string &&_name,
        std::gc_vector<std::gc_function<Block &()>> &&_funcs
    );
};

}
