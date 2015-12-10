#include "output.hpp"
#include "type.hpp"

namespace libblock {

OutputContext::OutputContext(std::ostream &_os):
    os {_os} {}

void OutputContext::tab() {
    os << "    ";
}

void OutputContext::enter() {
    ++indent;
}

void OutputContext::leave() {
    --indent;
}

void OutputContext::endl() {
    os << "\n";

    for (size_t to_print = indent; to_print > 0; --to_print) {
        tab();
    }
}

void OutputTask::insert(std::gc_function<void (OutputContext &)> &&render) {
    render_funcs.emplace_back(std::move(render));
}

void OutputTask::generate(OutputContext &oc) {
    for (std::gc_function<void (OutputContext &)> &render: render_funcs) {
        render(oc);
    }
}


void Output::header(
    Instance &instance,
    std::gc_function<void (OutputContext &)> &&render
) {
    headers.at(instance).get().insert(std::move(render));
}

void Output::content(
    Instance &instance,
    std::gc_function<void (OutputContext &)> &&render
) {
    contents.at(instance).get().insert(std::move(render));
}

void Output::insert(Instance &instance) {
    headers.insert({
        instance,
        *new (GC) OutputTask {}
    });
    contents.insert({
        instance,
        *new (GC) OutputTask {}
    });
}

void Output::getHeader(std::ostream &os, Instance &) const {
    OutputContext och {os};

    och.os << "/* #include \"lbstd.h\" */";
    och.endl();

    for (const auto &task: headers) {
        task.second.get().generate(och);
    }
}

void Output::getContent(std::ostream &os, Instance &root) const {
    OutputContext oc {os};

    // func_illegal

    oc.os << "void func_illegal() {";
    oc.enter();

        oc.endl();
        oc.os << "fprintf(stderr, \"bad func pointer\");";
        oc.endl();
        oc.os << "exit(1);";

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();

    // func exit

    oc.endl();
    oc.os << "void func_exit() {";
    oc.enter();

        oc.endl();
        oc.os << "exit(0);";

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();

    // exec

    for (const auto &task: contents) {
        task.second.get().generate(oc);
    }

    // start

    oc.endl();
    oc.os << "void _start() {";
    oc.enter();

        oc.endl();
        oc.os << "GC_init();";
        oc.endl();

        oc.endl();
        oc.os << "lb_func_t exit_p = &func_exit;";
        oc.endl();

        oc.endl();
        oc.os << root.strStruct() << " root;";
        oc.endl();
        oc.os << "root.frame.func = " << root.strFunc() << ";";
        oc.endl();
        oc.os << "root.frame.caller = (struct frame *) &exit_p;";
        oc.endl();

        oc.endl();
        oc.os << root.strFunc() << "(&root.frame, 0, 0, 0);";

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();
}

}
