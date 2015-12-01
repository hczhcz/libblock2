#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

NodeBlock::NodeBlock(
    Node *_source,
    std::list<std::unique_ptr<Block>> &&_blocks
):
    source_p {_source},
    blocks {std::move(_blocks)} {}

void NodeBlock::addBlock(Block *block_p) {
    blocks.push_back(std::unique_ptr<Block> {block_p});
}

void NodeBlock::buildProc(
    Instance &,
    Output &
) {
    throw ErrorDiscardNotAllowed {};
}

Type &NodeBlock::buildOut(
    Instance &instance,
    Output &output,
    std::function<std::string (Type &)> &&target
) {
    // get type

    Type &parent {
        source_p->buildOut(
            instance,
            output,
            std::move(target)
        )
    };

    Type &type {instance.addClosure(parent, *this)};

    // return

    return type;
}

void NodeBlock::buildIn(
    Instance &, Type &,
    Output &,
    std::function<std::string (Type &)> &&
) {
    throw ErrorWriteNotAllowed {};
}

}
