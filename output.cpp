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
    och.endl();

    for (const auto &task: headers) {
        task.second->generate(och);
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
        oc.os << "exit(1);"; // TODO

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();

    // func exit

    oc.endl();
    oc.os << "void func_exit() {";
    oc.enter();

        oc.endl();
        oc.os << "return;"; // TODO

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();

    // exec

    for (const auto &task: contents) {
        task.second->generate(oc);
    }

    // main

    oc.endl();
    oc.os << "int main() {";
    oc.enter();

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
        oc.endl();
        oc.endl();
        oc.os << "return 0;";

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();
}

}
