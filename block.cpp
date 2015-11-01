#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

Instance &Block::getInstance(Instance &&instance, Output &output) {
    for (Instance &target: instances) {
        bool ok {true};

        for (const auto &symbol: instance.symbol_types) {
            if (symbol.second != target.symbol_types.at(symbol.first)) {
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

    // TODO: render

    instances.push_back(std::move(instance));
    ast->buildProc(instances.back(), output);

    return instances.back();
}

void Block::buildProc(Instance &instance, Output &output) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // render

    uintptr_t id {tuid()};

    output.at(instance.tuid()).content
        << "    " << util::cType("result_" + std::to_string(nuid()), id)
        << " = " << util::cCode(id) << ";\n";

    // get type

    return *this;
}

void Block::buildIn(Instance &instance, Type &type, Output &output) {
    throw std::exception {};
}
