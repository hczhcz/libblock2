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

void NodeBlock::buildCall(
    Instance &caller,
    size_t position,
    Output &output,
    std::gc_function<void (Block &, Instance &)> &&before,
    std::gc_function<void (Block &, Instance &)> &&after
) {
    if (blocks.size() > 1) {
        Block *selected_p {nullptr};

        for (Block &block: blocks) {
            if (
                forkTry([&]() {
                    block.buildCall(
                        caller, position, output,
                        std::move(before),
                        std::move(after)
                    );
                })
            ) {
                if (selected_p) {
                    throw ErrorOverloadAmbiguous {};
                } else {
                    selected_p = &block;
                }
            }
        }

        if (selected_p) {
            selected_p->buildCall(
                caller, position, output,
                std::move(before),
                std::move(after)
            );
        } else {
            throw ErrorOverloadNotFound {};
        }
    } else if (blocks.size() == 1) {
        blocks.front().get().buildCall(
            caller, position, output,
            std::move(before),
            std::move(after)
        );
    } else {
        throw ErrorBlockNotFound {};
    }
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
