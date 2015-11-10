#include "output.hpp"
#include "type.hpp"

void OutputContext::enter() {
    ++indent;
}

void OutputContext::leave() {
    --indent;
}

void OutputContext::endl() {
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

void Output::getHeader(std::ostream &os, Instance &) const {
    os << "#include <stdlib.h>\n"
       << "#include <stdio.h>\n"
       << "#include <gc/gc.h>\n"
       << "\n"
       << "struct frame {\n"
       << "    void *func;\n"
       << "    struct frame *caller;\n"
       << "    struct frame *outer;\n"
       << "};\n";

    for (const auto &member: headers) {
        os << member.second->os.str();
    }
}

void Output::getContent(std::ostream &os, Instance &root) const {
    os << root.strStruct() << " root;\n"
          "\n"
          "int main() {\n"
          "    void *exit_addr = &&exit;\n"
          "\n"
          "    struct frame *self = &root.frame;\n"
          "    struct frame *callee = 0;\n"
          "    struct frame *inner = 0;\n"
          "    void *tmp = 0;\n"
          "\n"
          "    self->func = &&" << root.strFunc() << ";\n"
          "    self->caller = (struct frame *) &exit_addr;\n"
          "\n"
          "    goto *self->func;\n";

    for (const auto &member: contents) {
        os << member.second->os.str();
    }

    os << "\n"
          "    exit:\n"
          "    return 0;\n"
          "}\n";
}
