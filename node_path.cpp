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

    source->buildOut(instance, output, "???").at(name);

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << instance.strCast();
    renderPath(oc.os);
    oc.os << ";";
}

Type &NodePath::buildOut(
    Instance &instance,
    Output &output, const std::string &target
) {
    // get type

    Type &type {
        source->buildOut(instance, output, "???").at(name)
    };

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << target << " = " << instance.strCast();
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

    source->buildOut(instance, output, "???").insert(name, type);

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << instance.strCast();
    renderPath(oc.os);
    oc.os << " = " << target << ";";
}
