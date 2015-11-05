#include "node.hpp"

void TypeBlock::call(
    Instance &caller,
    std::vector<std::unique_ptr<Node>> &args,
    Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    block.build(
        output,
        [&](Instance &child) {
            child.insert("parent", parent);

            for (size_t i = 0; i < args.size(); ++i) {
                block.inArg(caller, child, i, args[i], output);
            }

            before(child);
        },
        [&](Instance &child) {
            after(child);

            for (size_t i = 0; i < args.size(); ++i) {
                block.outArg(caller, child, i, args[i], output);
            }
        }
    );
}

void TypeBlock::renderDecl(
    std::ostream &os,
    const std::string &name
) const {
    os << "struct obj_" << parent.tuid() << " *" << name;
}
