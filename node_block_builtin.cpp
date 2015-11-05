#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

void BlockBuiltin::buildContent(Instance &, Output &) {
    // nothing, by default
}

std::map<std::string, BlockBuiltin &> &BlockBuiltin::builtins() {
    static std::map<std::string, BlockBuiltin &> map;

    return map;
}

void BlockBuiltin::applyBuiltin(Instance &instance) {
    for (const auto &builtin: builtins()) {
        instance.children.push_back(std::unique_ptr<TypeBlock> {
            new TypeBlock {instance, builtin.second}
        });
        Type &type {*instance.children.back()};

        instance.insert(builtin.first, type);
    }
}
