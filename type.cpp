#include "exception.hpp"
#include "type.hpp"

namespace libblock {

static size_t tuid_init {0};

Type::Type():
    tuid {tuid_init++} {}

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
