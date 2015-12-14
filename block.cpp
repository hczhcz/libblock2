#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

void Block::renderFrame(
    Instance &caller,
    size_t position,
    OutputContext &oc
) const {
    switch (mode) {
        case FrameMode::static_global:
            oc.endl();
            oc.os << "static "
                  << caller.strCalleeType(position) << " "
                  << caller.strCalleeName(position) << ";";
            oc.endl();
            oc.os << "inner = (struct frame *) &"
                  << caller.strCalleeName(position) << ";";
            break;

        case FrameMode::static_local:
            oc.endl();
            oc.os << "static _Threal_local "
                  << caller.strCalleeType(position) << " "
                  << caller.strCalleeName(position) << ";";
            oc.endl();
            oc.os << "inner = (struct frame *) &"
                  << caller.strCalleeName(position) << ";";
            break;

        case FrameMode::dynamic_gc:
            oc.endl();
            oc.os << "inner = GC_malloc(sizeof("
                  << caller.strCalleeType(position)
                  << "));";
            break;

        case FrameMode::dynamic_free:
            oc.endl();
            oc.os << "inner = malloc(sizeof("
                  << caller.strCalleeType(position)
                  << "));";
            break;
    }
}

void Block::renderCall(
    Instance &caller, Instance &instance,
    size_t position,
    OutputContext &oc
) const {
    oc.endl();
    oc.os << "LB_FUNC(" << caller.strLabel(position) << ");";
    oc.endl();
    oc.os << "self->func = &" << caller.strLabel(position) << ";";

    // notice: reset callee->func
    oc.endl();
    oc.os << "callee->func = &" << instance.strFunc() << ";";

    oc.endl();
    oc.os << "callee->caller = self;";
    oc.endl();
    oc.os << "self = callee;";

    oc.endl();
    oc.os << "LB_YIELD(" << caller.strLabel(position) << ")";
}

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

    if (!getOption(BlockOption::manual_lock)) {
        instance.lock();
    }

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

Block::Block(
    // FrameMode _mode, // TODO
    std::set<BlockOption> &&_options,
    std::gc_vector<std::pair<std::string, ParamMode>> &&_params
):
    // mode {_mode},
    mode {FrameMode::dynamic_gc},
    options {std::move(_options)},
    params {std::move(_params)} {}

void Block::inSpecialArg(
    Instance &, Instance &,
    size_t, Node &,
    Output &,
    std::gc_function<std::string (Type &)> &&
) {
    throw ErrorTooManyArguments {}; // TODO: va_args?
}

void Block::outSpecialArg(
    Instance &, Instance &,
    size_t, Node &,
    Output &,
    std::gc_function<std::string (Type &)> &&
) {
    // nothing, by default // TODO: va_args?
}

bool Block::getOption(BlockOption option) {
    return options.find(option) != options.end();
}

void Block::inArg(
    Instance &caller, Instance &instance,
    size_t index, Node &arg,
    Output &output,
    std::gc_function<std::string (Type &)> &&target
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
                    return target(type) + "->data." + params[index].first;
                }
            )
        );
    }
}

void Block::outArg(
    Instance &caller, Instance &instance,
    size_t index, Node &arg,
    Output &output,
    std::gc_function<std::string (Type &)> &&target
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
                return target(type) + "->data." + params[index].first;
            }
        );
    }
}

void Block::build(
    Output &output,
    std::gc_function<void (Block &, Instance &)> &&before,
    std::gc_function<void (Block &, Instance &)> &&after
) {
    // init

    Instance &instance_early {
        *new (GC) Instance
    };

    // in

    before(*this, instance_early);

    // find or create instance

    Instance &instance {
        matchInstance(instance_early, output)
    };

    // out

    after(*this, instance);
}

}
