#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

std::map<std::string, std::shared_ptr<NodeBlock>> &Builtin::builtins() {
    static std::map<std::string, std::shared_ptr<NodeBlock>> map;

    return map;
}

void Builtin::apply(Instance &instance) {
    for (const auto &builtin: builtins()) {
        instance.insert(
            builtin.first,
            instance.addClosure(*builtin.second)
        );
    }
}
