#include "util.hpp"
#include "output.hpp"
#include "block.hpp"

Type &Instance::at(const std::string &name) {
    const auto &symbol = symbol_types.find(name);

    if (symbol != symbol_types.end()) {
        return symbol->second;
    } else {
        throw std::exception {};
    }
}

void Instance::insert(const std::string &name, Type &type) {
    const auto &symbol = symbol_types.find(name);

    if (symbol != symbol_types.end()) {
        if (symbol->second != type) {
            throw std::exception {};
        }
    } else {
        symbol_types.insert({name, type});
    }
}

Instance &Instance::lookup(const std::vector<std::string> &path) {
    Instance *instance_p {this};

    for (const std::string &name: path) {
        instance_p = dynamic_cast<Instance *>(
            &instance_p->at(name)
        );

        if (!instance_p) {
            throw std::exception {};
        }
    }

    return *instance_p;
}

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
        output.insert(tuid()).header
    };

    // build

    instances.push_back(std::move(instance));
    ast->buildProc(instances.back(), output);

    return instances.back();
}
