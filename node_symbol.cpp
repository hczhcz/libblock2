#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

void NodeSymbol::buildProc(Instance &instance, Output &output) {
    // render

    std::ostringstream &os {output.at(instance.tuid()).content};

    os << "self->";

    for (std::string &i: path) {
        os << i << "->";
    }

    os << name << ";\n";

    // get type

    instance.lookup(path).at(name);
}

Type &NodeSymbol::buildOut(Instance &instance, Output &output) {
    // render

    std::ostringstream &os {output.at(instance.tuid()).content};

    os << "    " << nuidOut() << " = self->"; // TODO: type?

    for (std::string &i: path) {
        os << i << "->";
    }

    os << name << ";\n";

    // get type

    return instance.lookup(path).at(name);
}

void NodeSymbol::buildIn(Instance &instance, Type &type, Output &output) {
    // render

    std::ostringstream &os {output.at(instance.tuid()).content};

    os << "    self->";

    for (std::string &i: path) {
        os << i << "->";
    }

    os << name << " = " << nuidIn() << ";\n"; // TODO: type?

    // set type

    instance.lookup(path).insert(name, type);
}
