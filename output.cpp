#include "output.hpp"
#include "type.hpp"

std::ostream &Output::osHeader(Instance &instance) {
    return members.at(instance.tuid())->header;
}

std::ostream &Output::osContent(Instance &instance) {
    return members.at(instance.tuid())->content;
}

void Output::insert(Instance &instance) {
    members.insert({
        instance.tuid(),
        std::make_shared<OutputContext>()
    });
}

void Output::getHeader(std::ostream &os) const {
    for (const auto &member: members) {
        os << member.second->header.str();
    }
}

void Output::getContent(std::ostream &os) const {
    for (const auto &member: members) {
        os << member.second->content.str();
    }
}
