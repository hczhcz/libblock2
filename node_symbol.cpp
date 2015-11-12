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

NodeSymbol::NodeSymbol(std::string &&_name, LookupMode _mode):
    name {std::move(_name)},
    mode {_mode} {}

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

    // render

    output.content(
        instance,
        [&, level](OutputContext &oc) {
            oc.endl();
            oc.os << instance.strCast("self");
            renderPath(oc.os, level);
            oc.os << ";";
        }
    );
}

Type &NodeSymbol::buildOut(
    Instance &instance,
    Output &output,
    std::function<std::string ()> &&target
) {
    // get type

    Type *type_p {nullptr}; // return value

    size_t level {0};

    if (mode == LookupMode::local) {
        type_p = &instance.at(name);
    } else {
        type_p = &instance.lookup(name, level);
    }

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

    return *type_p;
}

void NodeSymbol::buildIn(
    Instance &instance, Type &type,
    Output &output,
    std::function<std::string ()> &&target
) {
    // set type

    size_t level {0};

    if (mode == LookupMode::global) {
        instance.check(
            instance.lookup(name, level), type
        );
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
