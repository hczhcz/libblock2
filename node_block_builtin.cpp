#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::map<std::string, BlockBuiltin &> BlockBuiltin::builtins;

void BlockBuiltin::applyBuiltin(Instance &instance) {
    for (const auto &builtin: builtins) {
        instance.children.push_back({instance, builtin.second});
        Type &type {instance.children.back()};

        instance.insert(builtin.first, type);
    }
}

void BlockBuiltin::buildContent(Instance &instance, Output &output) {
}
