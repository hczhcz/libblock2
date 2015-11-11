#include "output.hpp"
#include "type.hpp"
#include "node.hpp"

void NodePath::renderPath(std::ostream &os) const {
    if (name != "self") {
        os << "->" << name;
    }
}

NodePath::NodePath(Node *_source, std::string &&_name):
    source_p {_source},
    name {_name} {}

void NodePath::buildProc(
    Instance &instance,
    Output &output
) {
    // gen type

    Instance &inner {
        source_p->buildOut(
            instance,
            output,
            []() {
                return "tmp";
            }
        ).prepareLookup()
    };

    instance.at(name);

    // render

    output.content(
        instance,
        [&](OutputContext &oc) {
            oc.endl();
            oc.os << inner.strCast("tmp");
            renderPath(oc.os);
            oc.os << ";";
        }
    );
}

Type &NodePath::buildOut(
    Instance &instance,
    Output &output,
    std::function<std::string ()> &&target
) {
    // get type

    Instance &inner {
        source_p->buildOut(
            instance,
            output,
            []() {
                return "tmp";
            }
        ).prepareLookup()
    };

    Type &type {
        instance.at(name)
    };

    // render

    output.content(
        instance,
        [&, target](OutputContext &oc) {
            oc.endl();
            oc.os << target() << " = " << inner.strCast("tmp");
            renderPath(oc.os);
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
    // set type

    Instance &inner {
        source_p->buildOut(
            instance,
            output,
            []() {
                return "tmp";
            }
        ).prepareLookup()
    };

    inner.insert(name, type);

    // render

    output.content(
        instance,
        [&, target](OutputContext &oc) {
            oc.endl();
            oc.os << inner.strCast("tmp");
            renderPath(oc.os);
            oc.os << " = " << target() << ";";
        }
    );
}
