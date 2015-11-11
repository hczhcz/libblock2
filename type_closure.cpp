#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

TypeClosure::TypeClosure(Instance &_parent, NodeBlock &_blocks):
    parent {_parent},
    blocks {_blocks} {}

void TypeClosure::call(
    Output &output,
    std::function<void (Instance &, Block &, Instance &)> &&before,
    std::function<void (Instance &, Block &)> &&after
) {
    Block &block {*blocks.blocks[0]}; // TODO

    block.build(
        output,
        [&](Instance &callee) {
            before(callee, block, parent);
        },
        [&](Instance &callee) {
            after(callee, block);
        }
    );
}

std::string TypeClosure::strDecl(const std::string &name) const {
    // type: struct <parent object> *
    return parent.strDecl(name);
}
