#include "node.hpp"


void TypeClosure::call(
    Output &output,
    std::function<void (Instance &, Block &, Instance &)> &&before,
    std::function<void (Instance &, Block &)> &&after
) {
    block.build(
        output,
        [&](Instance &child) {
            before(child, block, parent);
        },
        [&](Instance &child) {
            after(child, block);
        }
    );
}

std::string TypeClosure::strDecl(const std::string &name) const {
    // type: struct <parent object> *
    return parent.strDecl(name);
}
