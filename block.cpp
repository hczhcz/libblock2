#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

Instance &Block::matchInstance(
    std::unique_ptr<Instance> &&instance_p,
    Output &output
) {
    // find exist instance

    for (std::unique_ptr<Instance> &exist_p: instances) {
        if (instance_p->in(*exist_p)) {
            return *exist_p;
        }
    }

    // not found

    instances.push_back(std::move(instance_p));
    Instance &instance {*instances.back()};

    output.insert(instance);

    // render header

    output.header(
        instance,
        [&](OutputContext &och) {
            instance.renderStruct(och);
        }
    );

    // render (before body)

    output.content(
        instance,
        [&](OutputContext &oc) {
            instance.renderFuncHead(oc);
        }
    );

    // build

    buildContent(instance, output);

    // render (after body)

    output.content(
        instance,
        [&](OutputContext &oc) {
            instance.renderFuncTail(oc);
        }
    );

    // return

    return instance;
}

void Block::inSpecialArg(
    Instance &, Instance &,
    size_t, std::unique_ptr<Node> &,
    Output &,
    std::function<std::string (Type &)> &&
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}

void Block::outSpecialArg(
    Instance &, Instance &,
    size_t, std::unique_ptr<Node> &,
    Output &,
    std::function<std::string (Type &)> &&
) {
    // nothing, by default // TODO: va_args?
}

Block::Block(
    std::set<BlockOption> &&_options,
    std::vector<std::pair<std::string, ParamMode>> &&_params
):
    options {std::move(_options)},
    params {std::move(_params)} {}

bool Block::getOption(BlockOption option) {
    return options.find(option) != options.end();
}

void Block::inArg(
    Instance &caller, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output,
    std::function<std::string (Type &)> &&target
) {
    if (
        index >= params.size()
        || params[index].second == ParamMode::special
    ) {
        inSpecialArg(
            caller, instance,
            index, arg,
            output, std::move(target)
        );
    } else if (
        params[index].second == ParamMode::in
        || params[index].second == ParamMode::var
    ) {
        instance.insert(
            params[index].first,
            arg->buildOut(
                caller,
                output,
                [&, index, target = std::move(target)](Type &type) {
                    return target(type) + "->" + params[index].first;
                }
            )
        );
    }
}

void Block::outArg(
    Instance &caller, Instance &instance,
    size_t index, std::unique_ptr<Node> &arg,
    Output &output,
    std::function<std::string (Type &)> &&target
) {
    if (
        index >= params.size()
        || params[index].second == ParamMode::special
    ) {
        outSpecialArg(
            caller, instance,
            index, arg,
            output, std::move(target)
        );
    } else if (
        params[index].second == ParamMode::out
        || params[index].second == ParamMode::var
    ) {
        arg->buildIn(
            caller,
            instance.at(params[index].first),
            output,
            [&, index, target = std::move(target)](Type &type) {
                return target(type) + "->" + params[index].first;
            }
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
