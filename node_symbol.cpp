#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void NodeSymbol::buildProc(Instance &instance, Output &output) {
    // get type

    instance.lookup(path).at(name);

    // render

    std::ostringstream &os {output.at(instance.tuid()).content};

    os << "self->";

    for (std::string &i: path) {
        os << i << "->";
    }

    os << name << ";\n";
}

Type &NodeSymbol::buildOut(Instance &instance, Output &output) {
    // get type

    Type &type {instance.lookup(path).at(name)};

    // render

    std::ostringstream &os {output.at(instance.tuid()).content};

    os << "    " << type.renderDecl(nuidOut()) << " = self->";

    for (std::string &i: path) {
        os << i << "->";
    }

    os << name << ";\n";

    // return

    return type;
}

void NodeSymbol::buildIn(Instance &instance, Type &type, Output &output) {
    // set type

    instance.lookup(path).insert(name, type);

    // render

    std::ostringstream &os {output.at(instance.tuid()).content};

    os << "    self->";

    for (std::string &i: path) {
        os << i << "->";
    }

    os << name << " = " << nuidIn() << ";\n";
}
