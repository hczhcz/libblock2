#include "output.hpp"
#include "node.hpp"

void NodePath::renderPath(std::ostream &os) const {
    if (name != "self") {
        os << "->" << name;
    }
}

void NodePath::buildProc(
    Instance &instance,
    Output &output
) {
    // gen type

    Instance &child {
        source->buildOut(instance, output, "tmp").prepareLookup()
    };

    instance.at(name);

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << child.strCast("tmp");
    renderPath(oc.os);
    oc.os << ";";
}

Type &NodePath::buildOut(
    Instance &instance,
    Output &output, const std::string &target
) {
    // get type

    Instance &child {
        source->buildOut(instance, output, "tmp").prepareLookup()
    };

    Type &type {
        instance.at(name)
    };

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << target << " = " << child.strCast("tmp");
    renderPath(oc.os);
    oc.os << ";";

    // return

    return type;
}

void NodePath::buildIn(
    Instance &instance, Type &type,
    Output &output, const std::string &target
) {
    // set type

    Instance &child {
        source->buildOut(instance, output, "tmp").prepareLookup()
    };

    child.insert(name, type);

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << child.strCast("tmp");
    renderPath(oc.os);
    oc.os << " = " << target << ";";
}
