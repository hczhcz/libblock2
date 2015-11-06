#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::map<std::string, BlockBuiltin &> &BlockBuiltin::builtins() {
    static std::map<std::string, BlockBuiltin &> map;

    return map;
}

void BlockBuiltin::applyBuiltin(Instance &instance) {
    for (const auto &builtin: builtins()) {
        instance.insert(
            builtin.first,
            builtin.second.addClosureType(instance)
        );
    }
}
