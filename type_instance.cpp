#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

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
    oc.endl();
    oc.os << strFunc() << ":";
    oc.enter();
}

void Instance::renderFuncTail(OutputContext &oc) const {
    oc.endl();
    oc.os << "goto *self->caller->func;";

    oc.leave();
    oc.endl();
}

std::string Instance::strFunc() const {
    return "func_" + std::to_string(tuid());
}

std::string Instance::strStruct() const {
    return "struct struct_" + std::to_string(tuid());
}

std::string Instance::strCast(const std::string &name) const {
    return "((" + strStruct() + " *) " + name + ")";
}

std::string Instance::strLabel(const NodeCall &call) const {
    return "label_"
        + std::to_string(tuid()) + "_"
        + std::to_string(call.nuid());
}

std::string Instance::strCalleeType(const NodeCall &call) const {
    return callee_types.at(call.nuid()).strStruct();
}

std::string Instance::strCalleeName(const NodeCall &call) const {
    return "object_"
        + std::to_string(tuid()) + "_"
        + std::to_string(call.nuid());
}

std::string Instance::strInner(const NodeCall &call) const {
    return "((" + strCalleeType(call) + " *) inner)";
}

std::string Instance::strCallee(const NodeCall &call) const {
    return "((" + strCalleeType(call) + " *) callee)";
}

void Instance::check(Type &type1, Type &type2) {
    if (type1 != type2) {
        throw ErrorTypeCollision {};
    }
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
            check(symbol->second, type);
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

Type &Instance::addClosure(NodeBlock &blocks) {
    const auto &closure = closure_types.find(blocks.nuid());

    if (closure != closure_types.end()) {
        return *closure->second;
    } else {
        return *closure_types.insert({
            blocks.nuid(),
            std::make_shared<TypeClosure>(
                *this, blocks
            )
        }).first->second;
    }
}

void Instance::addCallee(NodeCall &call, Instance &callee) {
    callee_types.insert({call.nuid(), callee});
}

std::string Instance::strDecl(const std::string &name) const {
    return strStruct() + " *" + name;
}
