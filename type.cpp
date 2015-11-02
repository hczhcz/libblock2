#include "type.hpp"

uintptr_t Type::tuid() const {
    return (uintptr_t) this;
}

template <>
std::string TypeNative<bool>::renderDecl(std::string &&name) const {
    return "bool " + name;
}

template <>
std::string TypeNative<int64_t>::renderDecl(std::string &&name) const {
    return "int64_t " + name;
}

template <>
std::string TypeNative<double>::renderDecl(std::string &&name) const {
    return "double " + name;
}

template <>
std::string TypeNative<std::string>::renderDecl(std::string &&name) const {
    return "const char " + name + "[]"; // TODO: ptr?
}

std::string TypeBlock::renderDecl(std::string &&name) const {
    // return "block_" + std::to_string(tuid()) + " " + name;
    return "obj_" + std::to_string(parent.tuid()) + " *" + name;
}

Type &Instance::at(const std::string &name) {
    const auto &symbol = symbol_types.find(name);

    if (symbol != symbol_types.end()) {
        return symbol->second;
    } else {
        throw std::exception {};
    }
}

void Instance::insert(const std::string &name, Type &type) {
    const auto &symbol = symbol_types.find(name);

    if (symbol != symbol_types.end()) {
        if (symbol->second != type) {
            throw std::exception {};
        }
    } else {
        symbol_types.insert({name, type});
    }
}

Instance &Instance::lookup(const std::vector<std::string> &path) {
    Instance *instance_p {this};

    for (const std::string &name: path) {
        instance_p = dynamic_cast<Instance *>(
            &instance_p->at(name)
        );

        if (!instance_p) {
            throw std::exception {};
        }
    }

    return *instance_p;
}

std::string Instance::renderDecl(std::string &&name) const {
    return "obj_" + std::to_string(tuid()) + " *" + name;
}
