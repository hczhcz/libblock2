#include "exception.hpp"
#include "type.hpp"

static std::unique_ptr<std::nullptr_t> heap {
    new std::nullptr_t
};

uintptr_t Type::tuid() const {
    return (uintptr_t) this - (uintptr_t) heap.get();
}

Type &Type::at(const std::string &name) {
    throw ErrorLookupNotAllowed {};
}

void Type::insert(const std::string &name, Type &type) {
    throw ErrorLookupNotAllowed {};
}

Type &Type::lookup(const std::string &name, size_t &level) {
    throw ErrorLookupNotAllowed {};
}
