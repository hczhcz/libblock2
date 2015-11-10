#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

void BlockUser::buildContent(Instance &instance, Output &output) {
    // gen type

    ast->buildProc(instance, output);
}

BlockUser::BlockUser(
    std::vector<std::pair<std::string, SymbolMode>> &&_params,
    Node *_ast
):
    Block {std::move(_params)},
    ast {_ast} {}
