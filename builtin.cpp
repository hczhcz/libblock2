#include "builtin.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

void BuiltinContainer::apply(
    std::string &&package,
    Instance &instance
) {
    for (Builtin &builtin: Builtin::all().at(std::move(package))) {
        auto node = nodes.find(builtin.name);

        if (node == nodes.end()) {
            node = nodes.emplace(
                builtin.name,
                std::make_unique<NodeBlock>(
                    new NodeLiteral<lb_void_t> {{}}
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

std::map<
    std::string,
    std::list<std::reference_wrapper<Builtin>
>> &Builtin::all() {
    static std::map<
        std::string,
        std::list<std::reference_wrapper<Builtin>
    >> builtins;

    return builtins;
}

Builtin::Builtin(
    std::string &&package,
    std::string &&_name,
    std::vector<std::function<Block *()>> &&_funcs
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

        list->second.push_back(std::ref(*this));
    }

}
