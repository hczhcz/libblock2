#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

Instance &Block::getInstance(Instance &&instance, Output &output) {
    for (Instance &target: instances) {
        bool ok {true};

        for (const auto &symbol: instance.symbol_types) {
            if (symbol.second != target.lookup(symbol.first)) {
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
