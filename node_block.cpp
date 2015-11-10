#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

Type &NodeBlock::addClosureType(Instance &instance) {
    if (
        closure_types.find(instance.tuid()) != closure_types.end()
    ) {
        return *closure_types.at(instance.tuid());
    }

    return *closure_types.insert({
        instance.tuid(),
        std::make_shared<TypeClosure>(
            instance, *this
        )
    }).first->second;
}

void NodeBlock::buildProc(
    Instance &,
    Output &
) {
    // nothing
}

Type &NodeBlock::buildOut(
    Instance &instance,
    Output &output, const std::string &target
) {
    // get type

    Type &type {addClosureType(instance)};

    // render

    OutputContext &oc {output.content(instance)};

    oc.endl();
    oc.os << target << " = " << instance.strCast("self") << ";";

    // return

    return type;
}

void NodeBlock::buildIn(
    Instance &, Type &,
    Output &, const std::string &
) {
    throw ErrorWriteNotAllowed {};
}
