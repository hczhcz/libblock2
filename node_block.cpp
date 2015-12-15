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
    Session &session,
    Instance &caller,
    size_t position,
    std::gc_function<void (Block &, Instance &)> &&before,
    std::gc_function<void (Block &, Instance &)> &&after
) {
    if (blocks.size() > 1) {
        Block *selected_p {nullptr};

        for (Block &block: blocks) {
            if (
                forkTry([&]() {
                    block.buildCall(
                        session, caller, position,
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
                session, caller, position,
                std::move(before),
                std::move(after)
            );
        } else {
            throw ErrorOverloadNotFound {};
        }
    } else if (blocks.size() == 1) {
        blocks.front().get().buildCall(
            session, caller, position,
            std::move(before),
            std::move(after)
        );
    } else {
        throw ErrorBlockNotFound {};
    }
}

void NodeBlock::buildProc(
    Session &,
    Instance &
) {
    throw ErrorDiscardNotAllowed {};
}

Type &NodeBlock::buildOut(
    Session &session,
    Instance &instance,
    std::gc_function<std::string (Type &)> &&target
) {
    // get type

    Type &parent {
        source.buildOut(
            session, instance,
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
    Session &,
    Instance &, Type &,
    std::gc_function<std::string (Type &)> &&
) {
    throw ErrorWriteNotAllowed {};
}

}
