#include "builtin.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

void BuiltinContainer::apply(Instance &instance) {
    for (Builtin &builtin: Builtin::all()) {
        auto node = nodes.find(builtin.name);

        if (node == nodes.end()) {
            node = nodes.emplace(
                builtin.name,
                std::make_unique<NodeBlock>(
                    new NodeLiteral<std::nullptr_t> {nullptr}
                )
            ).first;

            instance.insert(
                builtin.name,
                instance.addClosure(TypeNativeVoid::get(), *node->second)
            );
        }

        for (std::function<Block *()> &func: builtin.funcs) {
            node->second->addBlock(func());
        }
    }
}

std::list<std::reference_wrapper<Builtin>> &Builtin::all() {
    static std::list<std::reference_wrapper<Builtin>> builtins;

    return builtins;
}

Builtin::Builtin(
    std::string &&_name,
    std::vector<std::function<Block *()>> &&_funcs
):
    name {std::move(_name)},
    funcs {std::move(_funcs)} {
        all().push_back(std::ref(*this));
    }

}
