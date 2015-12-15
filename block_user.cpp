#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

void BlockUser::buildContent(
    Session &session,
    Instance &instance
) {
    // gen type

    ast.buildProc(session, instance);
}

BlockUser::BlockUser(
    std::set<BlockOption> &&_options,
    std::gc_vector<std::pair<std::string, ParamMode>> &&_params,
    Node &_ast
):
    Block {std::move(_options), std::move(_params)},
    ast {_ast} {}

}
