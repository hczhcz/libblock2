#include "output.hpp"

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

}
