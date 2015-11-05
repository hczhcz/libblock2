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
    std::unique_ptr<Instance> &&instance_p,
    Output &output
) {
    for (std::unique_ptr<Instance> &target_p: instances) {
        bool ok {true};

        for (const auto &symbol: instance_p->symbol_types) {
            if (symbol.second != target_p->at(symbol.first)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            // found

            return *target_p;
        }
    }

    // not found

    instances.push_back(std::move(instance_p));
    Instance &instance {*instances.back()};

    // build

    output.insert(instance);
    buildContent(instance, output);

    // render header

    std::ostream &osh {output.at(instance).header};

    instance.renderStruct(osh);
    instance.renderFuncDecl(osh);
    osh << ";\n\n";

    return instance;
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
    // nothing, by default // TODO: va_args?
}

void Block::buildProc(Instance &, Output &) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // get type

    instance.children.push_back(std::unique_ptr<TypeBlock> {
        new TypeBlock {instance, *this}
    });
    Type &type {*instance.children.back()};

    // render

    std::ostream &os {output.at(instance).content};

    os << "    ";
    type.renderDecl(os, nuidOut());
    os << " = self;\n";

    // return

    return type;
}

void Block::buildIn(Instance &, Type &, Output &) {
    throw ErrorWriteNotAllowed {};
}
