#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

void BlockUser::buildContent(Instance &instance, Output &output) {
    // gen type

    ast_p->buildProc(instance, output);
}

BlockUser::BlockUser(
    std::set<BlockOption> &&_options,
    std::vector<std::pair<std::string, ParamMode>> &&_params,
    Node *_ast
):
    Block {std::move(_options), std::move(_params)},
    ast_p {_ast} {}

}
