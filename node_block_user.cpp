#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

void BlockUser::buildContent(Instance &instance, Output &output) {
    // gen type

    ast->buildProc(instance, output);
}
