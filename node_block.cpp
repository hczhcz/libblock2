#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

NodeBlock::NodeBlock(std::list<std::unique_ptr<Block>> &&_blocks):
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

    Type &type {instance.addClosure(*this)};

    // render

    output.content(
        instance,
        [&, target = std::move(target)](OutputContext &oc) {
            oc.endl();
            oc.os << target(type) << " = " << instance.strCast("self") << ";";
        }
    );

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
