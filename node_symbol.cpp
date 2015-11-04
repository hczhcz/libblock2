#include "output.hpp"
#include "node.hpp"

void NodeSymbol::renderPath(std::ostringstream &os) {
    os << "self";

    for (size_t i = 0; i < level; ++i) {
        os << "->parent";
    }

    for (std::string &i: path) {
        if (i != "self") {
            os << "->" << i;
        }
    }

    if (name != "self") {
        os << "->" << name;
    }
}

void NodeSymbol::buildProc(Instance &instance, Output &output) {
    // gen type

    instance.fullLookup(path, name, level);

    // render

    std::ostringstream &os {output.at(instance).content};

    os << "    ";
    renderPath(os);
    os << ";\n";
}

Type &NodeSymbol::buildOut(Instance &instance, Output &output) {
    // get type

    Type &type {instance.fullLookup(path, name, level)};

    // render

    std::ostringstream &os {output.at(instance).content};

    os << "    " << type.renderDecl(nuidOut()) << " = ";
    renderPath(os);
    os << ";\n";

    // return

    return type;
}

void NodeSymbol::buildIn(Instance &instance, Type &type, Output &output) {
    // set type

    instance.lookup(path).insert(name, type);

    // render

    std::ostringstream &os {output.at(instance).content};

    os << "    ";
    renderPath(os);
    os << " = " << nuidIn() << ";\n";
}
