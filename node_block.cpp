#include "util.hpp"
#include "output.hpp"
#include "node.hpp"

Instance &Block::getInstance(Instance &&instance, Output &output) {
    for (Instance &target: instances) {
        bool ok {true};

        for (const auto &symbol: instance.symbol_types) {
            if (symbol.second != target.at(symbol.first)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            // found

            return target;
        }
    }

    // not found

    // render

    std::ostringstream &os {
        output.insert(instance.tuid()).header
    };

    // build

    instances.push_back(std::move(instance));
    ast->buildProc(instances.back(), output);

    return instances.back();
}

void Block::buildProc(Instance &instance, Output &output) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // get type

    instance.children.push_back({instance, *this});
    Type &type {instance.children.back()};

    // render

    output.at(instance.tuid()).content
        << "    " << type.renderDecl(nuidOut())
        << " = self;\n";

    // return

    return type;
}

void Block::buildIn(Instance &instance, Type &type, Output &output) {
    throw std::exception {};
}
