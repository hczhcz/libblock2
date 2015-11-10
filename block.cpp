#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

Instance &Block::matchInstance(
    std::unique_ptr<Instance> &&instance_p,
    Output &output
) {
    for (std::unique_ptr<Instance> &exist_p: instances) {
        bool ok {true};

        for (const auto &symbol: instance_p->symbol_types) {
            if (symbol.second != exist_p->at(symbol.first)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            // found

            return *exist_p;
        }
    }

    // not found

    instances.push_back(std::move(instance_p));
    Instance &instance {*instances.back()};

    output.insert(instance);

    // render (before body)

    OutputContext &oc {output.content(instance)};

    instance.renderFuncHead(oc);

    // build

    buildContent(instance, output);

    // render header

    OutputContext &och {output.header(instance)};

    instance.renderStruct(och);

    // render (after body)

    instance.renderFuncTail(oc);

    // return

    return instance;
}

void Block::inSpecialArg(
    Instance &, Instance &,
    size_t, std::unique_ptr<Node> &,
    Output &, const std::string &
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}

void Block::outSpecialArg(
    Instance &, Instance &,
    size_t, std::unique_ptr<Node> &,
    Output &, const std::string &
) {
    // nothing, by default // TODO: va_args?
}

Block::Block(std::vector<std::pair<std::string, SymbolMode>> &&_params):
    params {std::move(_params)} {}

void Block::inArg(
    Instance &caller, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output, const std::string &target
) {
    if (
        index >= params.size()
        || params[index].second == SymbolMode::special
    ) {
        inSpecialArg(caller, instance, index, arg, output, target);
    } else if (
        params[index].second == SymbolMode::in
        || params[index].second == SymbolMode::var
    ) {
        instance.insert(
            params[index].first,
            arg->buildOut(
                caller,
                output, target + "->" + params[index].first // TODO: use callback?
            )
        );
    }
}

void Block::outArg(
    Instance &caller, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output, const std::string &target
) {
    if (
        index >= params.size()
        || params[index].second == SymbolMode::special
    ) {
        outSpecialArg(caller, instance, index, arg, output, target);
    } else if (
        params[index].second == SymbolMode::out
        || params[index].second == SymbolMode::var
    ) {
        arg->buildIn(
            caller,
            instance.at(params[index].first),
            output, target + "->" + params[index].first // TODO: use callback?
        );
    }
}

void Block::build(
    Output &output,
    std::function<void (Instance &)> &&before,
    std::function<void (Instance &)> &&after
) {
    // init

    std::unique_ptr<Instance> instance_p {
        new Instance {}
    };

    // in

    before(*instance_p);

    // find or create instance

    Instance &instance {
        matchInstance(std::move(instance_p), output)
    };

    // out

    after(instance);
}
