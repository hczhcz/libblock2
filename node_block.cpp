#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

NodeBlock::NodeBlock(
    Node &_source,
    std::gc_vector<std::reference_wrapper<Block>> &&_blocks
):
    source {_source},
    blocks {std::move(_blocks)} {}

void NodeBlock::addBlock(Block &block) {
    blocks.push_back(block);
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
    std::gc_function<std::string (Type &)> &&target
) {
    // get type

    Type &parent {
        source.buildOut(
            instance,
            output,
            std::move(target)
        )
    };

    Type &type {
        *new (GC) TypeClosure {
            parent, *this
        }
    };

    // return

    return type;
}

void NodeBlock::buildIn(
    Instance &, Type &,
    Output &,
    std::gc_function<std::string (Type &)> &&
) {
    throw ErrorWriteNotAllowed {};
}

}
