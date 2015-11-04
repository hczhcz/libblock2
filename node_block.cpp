#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

std::unique_ptr<Instance> Block::initInstance(Instance &parent) {
    std::unique_ptr<Instance> instance {
        new Instance {*this}
    };

    instance->insert("parent", parent);

    return std::move(instance);
}

void Block::inArg(
    Instance &parent, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    if (
        index >= params.size()
        || params[index].second == SymbolMode::special
    ) {
        inSpecialArg(parent, instance, index, arg, output);
    } else if (
        params[index].second == SymbolMode::in
        || params[index].second == SymbolMode::var
    ) {
        instance.insert(
            params[index].first,
            arg->buildOut(parent, output)
        );
    }
}

Instance &Block::matchInstance(
    std::unique_ptr<Instance> &&instance,
    Output &output
) {
    for (std::unique_ptr<Instance> &target: instances) {
        bool ok {true};

        for (const auto &symbol: instance->symbol_types) {
            if (symbol.second != target->at(symbol.first)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            // found

            return *target;
        }
    }

    // not found

    instances.push_back(std::move(instance));
    Instance &new_instance {*instances.back()};

    // build

    output.insert(new_instance);
    buildContent(new_instance, output);

    // render header

    std::ostringstream &osh {output.at(new_instance).header};

    new_instance.renderStruct(osh);
    new_instance.renderFuncDecl(osh);
    osh << ";\n";

    return new_instance;
}

void Block::outArg(
    Instance &parent, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    if (
        index >= params.size()
        || params[index].second == SymbolMode::special
    ) {
        outSpecialArg(parent, instance, index, arg, output);
    } else if (
        params[index].second == SymbolMode::out
        || params[index].second == SymbolMode::var
    ) {
        arg->buildIn(
            parent,
            instance.at(params[index].first),
            output
        );
    }
}

void Block::inSpecialArg(
    Instance &, Instance &,
    size_t, std::unique_ptr<Node> &,
    Output &
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}

void Block::outSpecialArg(
    Instance &, Instance &,
    size_t, std::unique_ptr<Node> &,
    Output &
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}

void Block::buildProc(Instance &, Output &) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // get type

    instance.children.push_back({instance, *this});
    Type &type {instance.children.back()};

    // render

    std::ostringstream &os {output.at(instance).content};

    os << "    ";
    type.renderDecl(os, nuidOut());
    os << " = self;\n";

    // return

    return type;
}

void Block::buildIn(Instance &, Type &, Output &) {
    throw ErrorWriteNotAllowed {};
}
