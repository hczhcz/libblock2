#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

void Block::inArg(
    Instance &caller, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    if (
        index >= params.size()
        || params[index].second == SymbolMode::special
    ) {
        inSpecialArg(caller, instance, index, arg, output);
    } else if (
        params[index].second == SymbolMode::in
        || params[index].second == SymbolMode::var
    ) {
        instance.insert(
            params[index].first,
            arg->buildOut(caller, output)
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

    std::ostream &osh {output.osHeader(instance)};

    instance.renderStruct(osh);
    instance.renderFuncDecl(osh);
    osh << ";\n\n";

    return instance;
}

void Block::outArg(
    Instance &caller, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output
) {
    if (
        index >= params.size()
        || params[index].second == SymbolMode::special
    ) {
        outSpecialArg(caller, instance, index, arg, output);
    } else if (
        params[index].second == SymbolMode::out
        || params[index].second == SymbolMode::var
    ) {
        arg->buildIn(
            caller,
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

void Block::buildBoot(
    Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // init

    std::unique_ptr<Instance> instance_p {
        new Instance {}
    };

    // in args

    before(*instance_p);

    // find or create instance

    Instance &instance {
        matchInstance(std::move(instance_p), output)
    };

    // out

    after(instance);
}

void Block::buildCall(
    Instance &parent, Instance &caller,
    std::vector<std::unique_ptr<Node>> &args,
    Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // init

    std::unique_ptr<Instance> instance_p {
        new Instance {}
    };

    instance_p->insert("parent", parent);

    // in args

    before(*instance_p);
    for (size_t i = 0; i < args.size(); ++i) {
        inArg(caller, *instance_p, i, args[i], output);
    }

    // find or create instance

    Instance &instance {
        matchInstance(std::move(instance_p), output)
    };

    // render

    std::ostream &os {output.osContent(caller)};

    os << "    ";
    os << "<TODO: call>;\n"; // TODO

    // out

    for (size_t i = 0; i < args.size(); ++i) {
        outArg(caller, instance, i, args[i], output);
    }
    after(instance);
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

    std::ostream &os {output.osContent(instance)};

    os << "    ";
    type.renderDecl(os, nuidOut());
    os << " = self;\n";

    // return

    return type;
}

void Block::buildIn(Instance &, Type &, Output &) {
    throw ErrorWriteNotAllowed {};
}
