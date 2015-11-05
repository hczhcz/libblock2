#include "output.hpp"
#include "type.hpp"

OutputContext &Output::at(Instance &instance) {
    return map.at(instance.tuid());
}

OutputContext &Output::insert(Instance &instance) {
    members.push_back(
        std::unique_ptr<OutputContext> {
            new OutputContext
        }
    );

    return map.insert({
        instance.tuid(), *members.back()
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
