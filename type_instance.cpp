#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"

std::string Instance::strFunc() const {
    return "func_" + std::to_string(tuid());
}

std::string Instance::strStruct() const {
    return "struct struct_" + std::to_string(tuid());
}

std::string Instance::strCast(const std::string &name) const {
    return "((" + strStruct() + " *) " + name + ")";
}

void Instance::renderStruct(OutputContext &oc) const {
    oc.endl();
    oc.os << strStruct() << " {";
    oc.enter();
    oc.endl();
    oc.os << "struct frame frame;";
    oc.endl();

    for (const auto &symbol: symbol_types) {
        oc.endl();
        oc.os << symbol.second.strDecl(symbol.first) << ";";
    }

    oc.leave();
    oc.endl();
    oc.os << "};";
    oc.endl();
}

void Instance::renderFuncHead(OutputContext &oc) const {
    oc.enter();
    oc.endl();
    oc.os << strFunc() << ":";
}

void Instance::renderFuncTail(OutputContext &oc) const {
    oc.endl();
    oc.os << "goto *self->caller->func;";
    oc.leave();
    oc.endl();
}

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

Type &Instance::lookup(const std::string &name, size_t &level) {
    if (name == "self") {
        return *this;
    } else {
        const auto &symbol = symbol_types.find(name);

        if (symbol != symbol_types.end()) {
            return symbol->second;
        } else {
            ++level;

            return at("parent").prepareLookup().lookup(name, level);
        }
    }
}

std::string Instance::strDecl(const std::string &name) const {
    return strStruct() + " *" + name;
}
