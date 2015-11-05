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
    return map.insert({
        instance.tuid(), *new OutputContext
    }).first->second;
}

void Output::getHeader(std::ostream &os) const {
    for (const auto &i: map) {
        os << i.second.header.str();
    }
}

void Output::getContent(std::ostream &os) const {
    for (const auto &i: map) {
        os << i.second.content.str();
    }
}
