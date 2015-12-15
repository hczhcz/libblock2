#include "builtin.hpp"
#include "type.hpp"
#include "node.hpp"

namespace libblock {

std::gc_map<
    std::string,
    std::gc_list<std::reference_wrapper<Builtin>>
> &Builtin::all() {
    static std::gc_map<
        std::string,
        std::gc_list<std::reference_wrapper<Builtin>>
    > builtins;

    return builtins;
}

void Builtin::apply(
    std::string &&package,
    Instance &instance
) {
    std::gc_map<
        std::string,
        std::reference_wrapper<NodeBlock>
    > nodes;

    for (Builtin &builtin: Builtin::all().at(std::move(package))) {
        auto node = nodes.find(builtin.name);

        if (node == nodes.end()) {
            node = nodes.insert({
                builtin.name,
                *new (GC) NodeBlock {
                    *new (GC) NodeLiteral<lb_void_t> {nullptr},
                    {}
                }
            }).first;

            instance.insert(
                builtin.name,
                *new (GC) TypeClosure {
                    TypeNativeVoid::get(),
                    node->second
                }
            );
        }

        for (std::gc_function<Block &()> &func: builtin.funcs) {
            node->second.get().addBlock(func());
        }
    }
}

Builtin::Builtin(
    std::string &&package,
    std::string &&_name,
    std::gc_vector<std::gc_function<Block &()>> &&_funcs
):
    name {std::move(_name)},
    funcs {std::move(_funcs)} {
        auto list = all().find(package);

        if (list == all().end()) {
            list = all().insert({
                std::move(package),
                {}
            }).first;
        }

        list->second.push_back(*this);
    }

}
