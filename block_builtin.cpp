#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

std::map<std::string, std::shared_ptr<NodeBlock>> BuiltinManager::builtins() {
    static std::map<std::string, std::shared_ptr<NodeBlock>> map;

    return map;
}

void BuiltinManager::apply(Instance &instance) {
    for (const auto &builtin: builtins()) {
        instance.insert(
            builtin.first,
            builtin.second->addClosureType(instance)
        );
    }
}
