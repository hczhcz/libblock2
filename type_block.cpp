#include "node.hpp"

void TypeBlock::call(
    Instance &caller,
    std::vector<std::unique_ptr<Node>> &args,
    Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    block.buildCall(
        parent, caller, args, output,
        std::move(before),
        std::move(after)
    );
}

void TypeBlock::renderDecl(
    std::ostream &os,
    const std::string &name
) const {
    os << "struct obj_" << parent.tuid() << " *" << name;
}
