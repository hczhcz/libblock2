#include "output.hpp"
#include "type.hpp"

namespace libblock {

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
    headers.at(std::ref(instance))->insert(std::move(render));
}

void Output::content(
    Instance &instance,
    std::function<void (OutputContext &)> &&render
) {
    contents.at(std::ref(instance))->insert(std::move(render));
}

void Output::insert(Instance &instance) {
    headers.emplace(
        std::ref(instance),
        std::make_unique<OutputTask>()
    );
    contents.emplace(
        std::ref(instance),
        std::make_unique<OutputTask>()
    );
}

void Output::getHeader(std::ostream &os, Instance &) const {
    OutputContext och {os};

    och.os << "/* #include \"lbstd.h\" */";

    for (const auto &task: headers) {
        task.second->generate(och);
    }
}

void Output::getContent(std::ostream &os, Instance &root) const {
    OutputContext oc {os};

    // exec

    oc.os << "void exec(struct frame *frame, size_t func) {";
    oc.enter();

        oc.endl();
        oc.os << "lb_func_t exports[] = {&&global_exit, &&" << root.strFunc() << "};";
        oc.endl();
        oc.endl();
        oc.os << "struct frame *self = frame;";
        oc.endl();
        oc.os << "struct frame *callee = 0;";
        oc.endl();
        oc.os << "struct frame *inner = 0;";
        // notice: type of tmp may change if 128-bit values are supported
        oc.endl();
        oc.os << "lb_reg_t tmp = 0;";
        oc.endl();
        oc.endl();
        oc.os << "self->func = exports[func];";
        oc.endl();
        oc.os << "self->caller = (struct frame *) &exports[0];";
        oc.endl();
        oc.os << "goto *self->func;";
        oc.endl();

        for (const auto &task: contents) {
            task.second->generate(oc);
        }

        oc.endl();
        oc.os << "global_func_error:";
        oc.enter();

            oc.endl();
            oc.os << "printf(\"bad func pointer\");";
            oc.endl();
            oc.os << "return;";

        oc.leave();
        oc.endl();

        oc.endl();
        oc.os << "global_exit:";
        oc.enter();

            oc.endl();
            oc.os << "return;";

        oc.leave();

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();

    // main

    oc.endl();
    oc.os << "int main() {";
    oc.enter();

        oc.endl();
        oc.os << root.strStruct() << " root;";
        oc.endl();
        oc.os << "exec(&root.frame, 1);";
        oc.endl();
        oc.endl();
        oc.os << "return 0;";

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();
}

}
