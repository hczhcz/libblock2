#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

void NodePath::renderPath(std::ostream &os, size_t level) const {
    for (size_t i = 0; i < level; ++i) {
        os << "->parent";
    }

    if (name != "self") {
        os << "->" << name;
    }
}

NodePath::NodePath(Node *_source, LookupMode _mode, std::string &&_name):
    source_p {_source},
    mode {_mode},
    name {_name} {}

void NodePath::buildProc(
    Instance &instance,
    Output &output
) {
    // get inner

    Instance &inner {
        source_p->buildOut(
            instance,
            output,
            []() -> std::string {
                return "tmp";
            }
        ).prepareLookup()
    };

    // gen type

    size_t level {0};

    if (mode == LookupMode::local) {
        inner.at(name);
    } else {
        inner.lookup(name, level);
    }

    // render

    output.content(
        instance,
        [&, level](OutputContext &oc) {
            oc.endl();
            oc.os << inner.strCast("tmp");
            renderPath(oc.os, level);
            oc.os << ";";
        }
    );
}

Type &NodePath::buildOut(
    Instance &instance,
    Output &output,
    std::function<std::string ()> &&target
) {
    // get inner

    Instance &inner {
        source_p->buildOut(
            instance,
            output,
            []() -> std::string {
                return "tmp";
            }
        ).prepareLookup()
    };

    // get type

    size_t level {0};

    Type &type {
        mode == LookupMode::local ?
        inner.at(name) : inner.lookup(name, level)
    };

    // render

    output.content(
        instance,
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << target() << " = " << inner.strCast("tmp");
            renderPath(oc.os, level);
            oc.os << ";";
        }
    );

    // return

    return type;
}

void NodePath::buildIn(
    Instance &instance, Type &type,
    Output &output,
    std::function<std::string ()> &&target
) {
    // get inner

    Instance &inner {
        source_p->buildOut(
            instance,
            output,
            []() -> std::string {
                return "tmp";
            }
        ).prepareLookup()
    };

    // set type

    size_t level {0};

    if (mode == LookupMode::global) {
        inner.lookupCheck(name, type, level);
    } else {
        inner.insert(name, type);
    }

    // render

    output.content(
        instance,
        [&, target = std::move(target), level](OutputContext &oc) {
            oc.endl();
            oc.os << inner.strCast("tmp");
            renderPath(oc.os, level);
            oc.os << " = " << target() << ";";
        }
    );
}
