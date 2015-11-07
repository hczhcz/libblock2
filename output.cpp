#include "output.hpp"
#include "type.hpp"

void OutputContext::endl(size_t offset) {
    indent += offset;

    os << "\n";

    for (size_t to_print = indent; to_print > 0; --to_print) {
        os << "    ";
    }
}

OutputContext &Output::header(Instance &instance) {
    return *headers.at(instance.tuid());
}

OutputContext &Output::content(Instance &instance) {
    return *contents.at(instance.tuid());
}

void Output::insert(Instance &instance) {
    headers.insert({
        instance.tuid(),
        std::make_shared<OutputContext>()
    });
    contents.insert({
        instance.tuid(),
        std::make_shared<OutputContext>()
    });
}

void Output::getHeader(std::ostream &os) const {
    for (const auto &member: headers) {
        os << member.second->os.str();
    }
}

void Output::getContent(std::ostream &os) const {
    for (const auto &member: contents) {
        os << member.second->os.str();
    }
}
