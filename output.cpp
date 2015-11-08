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

void Output::getHeader(std::ostream &os, Instance &root) const {
    os << "#include <stdlib.h>\n"
       << "#include <stdio.h>\n"
       << "#include <gc/gc.h>\n"
       << "\n"
       << "typedef " << root.strStruct() << " frame_root;\n";

    for (const auto &member: headers) {
        os << member.second->os.str();
    }
}

void Output::getContent(std::ostream &os, Instance &root) const {
    os << "frame_root root;\n"
          "\n"
          "int main() {\n"
          "    void *exit_addr = &&exit;\n"
          "\n"
          "    root.func = &&" << root.strFunc() << ";\n"
          "    root.caller = &exit_addr;\n"
          "\n"
          "    void *self = &root;\n"
          "    void *callee = 0;\n"
          "\n"
          "    goto **(void ***) self;\n";

    for (const auto &member: contents) {
        os << member.second->os.str();
    }

    os << "\n"
          "    exit:\n"
          "    return 0;\n"
          "}\n";
}
