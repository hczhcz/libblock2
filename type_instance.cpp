#include "exception.hpp"
#include "type.hpp"

Type &Instance::at(const std::string &name) {
    if (name == "self") {
        return *this;
    } else {
        const auto &symbol = symbol_types.find(name);

        if (symbol != symbol_types.end()) {
            return symbol->second;
        } else {
            throw ErrorSymbolNotFound {};
        }
    }
}

void Instance::insert(const std::string &name, Type &type) {
    if (name == "self") {
        throw ErrorWriteNotAllowed {}; // TODO: really?
    } else {
        const auto &symbol = symbol_types.find(name);

        if (symbol != symbol_types.end()) {
            if (symbol->second != type) {
                throw ErrorTypeCollision {};
            }
        } else {
            symbol_types.insert({name, type});
        }
    }
}

Instance &Instance::lookup(const std::vector<std::string> &path) {
    Instance *instance_p {this};

    for (const std::string &name: path) {
        instance_p = dynamic_cast<Instance *>(
            &instance_p->at(name)
        );

        if (!instance_p) {
            throw ErrorLookupNotAllowed {};
        }
    }

    return *instance_p;
}

Type &Instance::fullLookup(
    const std::vector<std::string> &path,
    const std::string &name,
    size_t &level
) {
    try {
        return lookup(path).at(name);
    } catch (const ErrorSymbolNotFound &e) {
        try {
            Instance *instance_p {
                dynamic_cast<Instance *>(&at("parent"))
            };

            if (!instance_p) {
                throw ErrorLookupNotAllowed {};
            } else {
                level += 1;
                return instance_p->fullLookup(path, name, level);
            }
        } catch (const ErrorSymbolNotFound &e1) {
            throw e;
        }
    }
}