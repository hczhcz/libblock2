#include "output.hpp"

Output::~Output() {
    for (auto &i: map) {
        delete &i.second;
    }
}

OutputContext &Output::at(uintptr_t key) {
    return map.at(key);
}

OutputContext &Output::insert(uintptr_t key) {
    return map.insert({key, *new OutputContext}).first->second;
}
