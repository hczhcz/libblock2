#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

Instance Block::initInstance(Instance &parent) {
    Instance instance {*this};

    instance.insert("parent", parent);

    return instance;
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

Instance &Block::matchInstance(Instance &&instance, Output &output) {
    for (Instance &target: instances) {
        bool ok {true};

        for (const auto &symbol: instance.symbol_types) {
            if (symbol.second != target.at(symbol.first)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            // found

            return target;
        }
    }

    // not found

    instances.push_back(std::move(instance));
    Instance &new_instance {instances.back()};

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

std::map<std::string, BlockBuiltin &> BlockBuiltin::builtins;

void BlockBuiltin::applyBuiltin(Instance &instance) {
    for (const auto &builtin: builtins) {
        instance.children.push_back({instance, builtin.second});
        Type &type {instance.children.back()};

        instance.insert(builtin.first, type);
    }
}

void BlockBuiltin::inSpecialArg(
    Instance &parent, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    // TODO
}

void BlockBuiltin::buildContent(Instance &instance, Output &output) {
    // render (before body)

    output.at(instance).header
        << "builtin_" << name
        << "(block_" << instance.tuid() << " *self);\n";

    // gen type

    func(instance);
}

void BlockBuiltin::outSpecialArg(
    Instance &parent, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    // TODO
}

void BlockUser::inSpecialArg(
    Instance &parent, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}

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

void BlockUser::outSpecialArg(
    Instance &parent, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}
