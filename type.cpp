#include "exception.hpp"
#include "type.hpp"

static std::unique_ptr<std::nullptr_t> heap {
    new std::nullptr_t
};

uintptr_t Type::tuid() const {
    return ((uintptr_t) this) - ((uintptr_t) heap.get());
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
