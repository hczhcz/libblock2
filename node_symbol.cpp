#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

void NodeSymbol::renderPath(std::ostream &os, size_t level) const {
    for (size_t i = 0; i < level; ++i) {
        os << "->parent";
    }

    if (name != "self") {
        os << "->" << name;
    }
}

NodeSymbol::NodeSymbol(LookupMode _mode, std::string &&_name):
    mode {_mode},
    name {std::move(_name)} {}

void NodeSymbol::buildProc(
    Instance &instance,
    Output &output
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
    Instance &instance,
    Output &output,
    std::function<std::string ()> &&target
) {
    // get type

    size_t level {0};

    Type &type {
        mode == LookupMode::local ?
        instance.at(name) : instance.lookup(name, level)
    };

    // render

    output.content(
        instance,
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << target() << " = " << instance.strCast("self");
            renderPath(oc.os, level);
            oc.os << ";";
        }
    );

    // return

    return type;
}

void NodeSymbol::buildIn(
    Instance &instance, Type &type,
    Output &output,
    std::function<std::string ()> &&target
) {
    // set type

    size_t level {0};

    if (mode == LookupMode::global) {
        instance.lookupCheck(name, type, level);
    } else {
        instance.insert(name, type);
    }

    // render

    output.content(
        instance,
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << instance.strCast("self");
            renderPath(oc.os, level);
            oc.os << " = " << target() << ";";
        }
    );
}
