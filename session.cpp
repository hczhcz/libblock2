#include "session.hpp"
#include "output.hpp"
#include "type.hpp"

namespace libblock {

void Session::insert(Instance &instance) {
    instances.push_back(instance);
}


void Session::each(std::gc_function<void (Instance &)> &&func) {
    for (Instance &instance: instances) {
        func(instance);
    }
}

void Session::renderHeader(std::ostream &os, Instance &) const {
    OutputContext och {os};

    och.os << "/* #include \"lbstd.h\" */";
    och.endl();

    for (Instance &instance: instances) {
        instance.header.generate(och);
    }
}

void Session::renderContent(std::ostream &os, Instance &root) const {
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

    for (Instance &instance: instances) {
        instance.content.generate(oc);
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
        oc.os << root.strFunc() << "(&root.frame, 0, 0);";

    oc.leave();
    oc.endl();
    oc.os << "}";
    oc.endl();
}

}
