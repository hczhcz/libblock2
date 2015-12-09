#include "exception.hpp"
#include "type.hpp"

namespace libblock {

static std::nullptr_t &heap {
    *new (GC) std::nullptr_t
};

uintptr_t Type::tuid() const {
    return ((uintptr_t) this) - ((uintptr_t) &heap);
}

Instance &Type::prepareLookup() {
    Instance *instance_p {
        dynamic_cast<Instance *>(this)
    };

    if (instance_p) {
        return *instance_p;
    } else {
        throw ErrorLookupNotAllowed {};
    }
}

}
