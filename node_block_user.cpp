#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

void BlockUser::buildContent(Instance &instance, Output &output) {
    // render (before body)

    std::ostringstream &os {output.at(instance).content};

    instance.renderFuncDecl(os);
    os << " {\n";

    // gen type

    ast->buildProc(instance, output);

    // render (after body)

    os << "}\n\n";
}
