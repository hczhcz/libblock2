#include "node.hpp"

void TypeBlock::call(
    Output &output,
    std::function<void (Instance &, Block &)> &&before,
    std::function<void (Instance &, Block &)> &&after
) {
    block.build(
        output,
        [&](Instance &child) {
            child.insert("parent", parent);

            before(child, block);
        },
        [&](Instance &child) {
            after(child, block);
        }
    );
}

std::string TypeBlock::decl(const std::string &name) const {
    // type: struct <parent object> *
    return parent.decl(name);
}
