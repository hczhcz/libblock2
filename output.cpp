#include "output.hpp"
#include "type.hpp"

std::ostream &Output::osHeader(Instance &instance) {
    return map.at(instance.tuid()).header;
}

std::ostream &Output::osContent(Instance &instance) {
    return map.at(instance.tuid()).content;
}

void Output::insert(Instance &instance) {
    members.push_back(
        std::unique_ptr<OutputContext> {
            new OutputContext
        }
    );

    map.insert({
        instance.tuid(), *members.back()
    });
}

void Output::getHeader(std::ostream &os) const {
    for (const auto &member: map) {
        os << member.second.header.str();
    }
}

void Output::getContent(std::ostream &os) const {
    for (const auto &member: map) {
        os << member.second.content.str();
    }
}
