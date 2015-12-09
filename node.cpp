#include "node.hpp"

namespace libblock {

static std::nullptr_t &heap {
    *new (GC) std::nullptr_t
};

uintptr_t Node::nuid() const {
    return ((uintptr_t) this) - ((uintptr_t) &heap);
}

}
