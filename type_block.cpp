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

std::string TypeBlock::decl(const std::string &name) const {
    // type: struct <parent object> *
    return parent.decl(name);
}
