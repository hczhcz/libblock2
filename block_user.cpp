#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

void BlockUser::buildContent(Instance &instance, Output &output) {
    // gen type

    ast_p->buildProc(instance, output);
}

BlockUser::BlockUser(
    std::bitset<(size_t) BlockOption::END> &&_options,
    std::vector<std::pair<std::string, ParamMode>> &&_params,
    Node *_ast
):
    Block {std::move(_options), std::move(_params)},
    ast_p {_ast} {}
