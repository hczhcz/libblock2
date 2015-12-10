#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

Instance &Block::matchInstance(
    Instance &instance,
    Output &output
) {
    // find exist instance

    for (Instance &exist: instances) {
        if (instance.in(exist)) {
            return exist;
        }
    }

    // not found

    instances.push_back(instance);
    output.insert(instance);

    // render (before body)

    output.content(
        instance,
        [&](OutputContext &oc) {
            instance.renderFuncHead(oc);
        }
    );

    // build

    buildContent(instance, output);
    instance.lock(); // TODO: really?

    // render (after body)

    output.content(
        instance,
        [&](OutputContext &oc) {
            instance.renderFuncTail(oc);
        }
    );

    // render header

    output.header(
        instance,
        [&](OutputContext &och) {
            instance.renderFuncDef(och);
            instance.renderStruct(och);
        }
    );

    // return

    return instance;
}

void Block::inSpecialArg(
    Instance &, Instance &,
    size_t, Node &,
    Output &,
    std::function<std::string (Type &)> &&
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}

void Block::outSpecialArg(
    Instance &, Instance &,
    size_t, Node &,
    Output &,
    std::function<std::string (Type &)> &&
) {
    // nothing, by default // TODO: va_args?
}

Block::Block(
    std::set<BlockOption> &&_options,
    std::gc_vector<std::pair<std::string, ParamMode>> &&_params
):
    options {std::move(_options)},
    params {std::move(_params)} {}

bool Block::getOption(BlockOption option) {
    return options.find(option) != options.end();
}

void Block::inArg(
    Instance &caller, Instance &instance,
    size_t index, Node &arg,
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
            arg.buildOut(
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
    size_t index, Node &arg,
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
        arg.buildIn(
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

    Instance &instance_early {
        *new (GC) Instance
    };

    // in

    before(instance_early);

    // find or create instance

    Instance &instance {
        matchInstance(instance_early, output)
    };

    // out

    after(instance);
}

}
