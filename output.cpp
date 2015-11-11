#include "output.hpp"
#include "type.hpp"

OutputContext::OutputContext(std::ostream &_os):
    os {_os} {}

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

void OutputTask::insert(std::function<void (OutputContext &)> &&render) {
    render_funcs.push_back(std::move(render));
}

void OutputTask::generate(OutputContext &oc) {
    for (std::function<void (OutputContext &)> &render: render_funcs) {
        render(oc);
    }
}


void Output::header(
    Instance &instance,
    std::function<void (OutputContext &)> &&render
) {
    headers.at(instance.tuid())->insert(std::move(render));
}

void Output::content(
    Instance &instance,
    std::function<void (OutputContext &)> &&render
) {
    contents.at(instance.tuid())->insert(std::move(render));
}

void Output::insert(Instance &instance) {
    headers.insert({
        instance.tuid(),
        std::make_shared<OutputTask>()
    });
    contents.insert({
        instance.tuid(),
        std::make_shared<OutputTask>()
    });
}

void Output::getHeader(std::ostream &os, Instance &) const {
    OutputContext oc {os};

    oc.os << "#include <stdlib.h>\n"
             "#include <stdio.h>\n"
             "#include <gc/gc.h>\n"
             "\n"
             "struct frame {\n"
             "    void *func;\n"
             "    struct frame *caller;\n"
             "    struct frame *outer;\n"
             "};\n";

    for (const auto &task: headers) {
        task.second->generate(oc);
    }
}

void Output::getContent(std::ostream &os, Instance &root) const {
    OutputContext oc {os};

    oc.os << root.strStruct()
          << " root;\n"
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

    for (const auto &task: contents) {
        task.second->generate(oc);
    }

    oc.os << "\n"
             "    exit:\n"
             "    return 0;\n"
             "}\n";
}
