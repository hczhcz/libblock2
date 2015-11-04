#include "output.hpp"
#include "type.hpp"

Output::~Output() {
    for (auto &i: map) {
        delete &i.second;
    }
}

OutputContext &Output::at(Instance &instance) {
    return map.at(instance.tuid());
}

OutputContext &Output::insert(Instance &instance) {
    return map.insert({instance.tuid(), *new OutputContext}).first->second;
}
