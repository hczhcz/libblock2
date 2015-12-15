#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

namespace libblock {

void NodeSymbol::renderPath(std::ostream &os, size_t level) const {
    for (size_t i = 0; i < level; ++i) {
        os << "->data.parent";
    }

    if (name != "self") {
        os << "->data." << name;
    }
}

NodeSymbol::NodeSymbol(LookupMode _mode, std::string &&_name):
    mode {_mode},
    name {std::move(_name)} {}

NodeSymbol::NodeSymbol(LookupMode _mode, const std::string &_name):
    mode {_mode},
    name {_name} {}

void NodeSymbol::buildProc(
    Session &,
    Instance &instance
) {
    // gen type

    size_t level {0};

    if (mode == LookupMode::local) {
        instance.at(name);
    } else {
        instance.lookup(name, level);
    }
}

Type &NodeSymbol::buildOut(
    Session &,
    Instance &instance,
    std::gc_function<std::string (Type &)> &&target
) {
    // get type

    size_t level {0};

    Type &type {
        mode == LookupMode::local ?
        instance.at(name) : instance.lookup(name, level)
    };

    // render

    instance.content.insert(
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << target(type) << " = " << instance.strSelf();
            renderPath(oc.os, level);
            oc.os << ";";
        }
    );

    // return

    return type;
}

void NodeSymbol::buildIn(
    Session &,
    Instance &instance, Type &type,
    std::gc_function<std::string (Type &)> &&target
) {
    // set type

    size_t level {0};

    if (mode == LookupMode::global) {
        instance.lookupCheck(name, type, level);
    } else {
        instance.insert(name, type);
    }

    // render

    instance.content.insert(
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << instance.strSelf();
            renderPath(oc.os, level);
            oc.os << " = " << target(type) << ";";
        }
    );
}

}
