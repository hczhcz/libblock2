#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

namespace libblock {

Instance &NodePath::getInner(
    Session &session,
    Instance &instance
) {
    return source.buildOut(
        session, instance,
        [](Type &type) -> std::string {
            return type.strReint("tmp");
        }
    ).prepareLookup();
}

void NodePath::renderPath(std::ostream &os, size_t level) const {
    for (size_t i = 0; i < level; ++i) {
        os << "->data.parent";
    }

    if (name != "self") {
        os << "->data." << name;
    }
}

NodePath::NodePath(Node &_source, LookupMode _mode, std::string &&_name):
    source {_source},
    mode {_mode},
    name {std::move(_name)} {}

NodePath::NodePath(Node &_source, LookupMode _mode, const std::string &_name):
    source {_source},
    mode {_mode},
    name {_name} {}

void NodePath::buildProc(
    Session &session,
    Instance &instance
) {
    // get inner

    Instance &inner {
        getInner(session, instance)
    };

    // gen type

    size_t level {0};

    if (mode == LookupMode::local) {
        inner.at(name);
    } else {
        inner.lookup(name, level);
    }
}

Type &NodePath::buildOut(
    Session &session,
    Instance &instance,
    std::gc_function<std::string (Type &)> &&target
) {
    // get inner

    Instance &inner {
        getInner(session, instance)
    };

    // get type

    size_t level {0};

    Type &type {
        mode == LookupMode::local ?
        inner.at(name) : inner.lookup(name, level)
    };

    // render

    instance.content.insert(
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << target(type) << " = " << inner.strReint("tmp");
            renderPath(oc.os, level);
            oc.os << ";";
        }
    );

    // return

    return type;
}

void NodePath::buildIn(
    Session &session,
    Instance &instance, Type &type,
    std::gc_function<std::string (Type &)> &&target
) {
    // get inner

    Instance &inner {
        getInner(session, instance)
    };

    // set type

    size_t level {0};

    if (mode == LookupMode::global) {
        inner.lookupCheck(name, type, level);
    } else {
        inner.insert(name, type);
    }

    // render

    instance.content.insert(
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << inner.strReint("tmp");
            renderPath(oc.os, level);
            oc.os << " = " << target(type) << ";";
        }
    );
}

}
