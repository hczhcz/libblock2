#include "node.hpp"

static std::unique_ptr<std::nullptr_t> heap {
    new std::nullptr_t
};

uintptr_t Node::nuid() const {
    return ((uintptr_t) this) - ((uintptr_t) heap.get());
}
