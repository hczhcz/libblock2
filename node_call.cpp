#include "exception.hpp"
#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace libblock {

void NodeCall::build(
    Session &session,
    Instance &instance,
    std::gc_function<void (Block &)> &&init,
    std::gc_function<void (Instance &, size_t)> &&before,
    std::gc_function<void (Instance &, size_t)> &&after
) {
    // get callee

    Type &callee_type {
        source.buildOut(
            session, instance,
            [](Type &type) -> std::string {
                return type.strReint("tmp");
            }
        )
    };

    if (
        TypeClosure *closure_p {
            dynamic_cast<TypeClosure *>(&callee_type)
        }
    ) {
        size_t position {instance.addPosition()};

        // callee is closure
        // create a frame and call the function

        Type &parent {closure_p->parent};

        // render (enter)

        instance.content.insert(
            [&](OutputContext &oc) {
                oc.endl();
                oc.os << "/* call */";
                oc.enter();
            }
        );

        closure_p->blocks.buildCall(
            session, instance, position,
            [&](Block &block, Instance &callee) {
                // init

                init(block);

                // parent

                callee.insert("parent", parent);

                // render (parent)

                instance.content.insert(
                    [&, position](OutputContext &oc) {
                        oc.endl();
                        oc.os << instance.strInner(position) << "->data.parent = "
                              << parent.strReint("tmp") << ";";
                    }
                );

                // input

                before(callee, position);

                // render (load the callee)

                instance.content.insert(
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << "inner->outer = callee;";
                        oc.endl();
                        oc.os << "callee = inner;";
                    }
                );

                // in args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.inArg(
                        session,
                        instance, callee,
                        position, i, args[i]
                    );
                }
            },
            [&](Block &block, Instance &callee) {
                instance.addCallee(position, callee);

                // out args

                for (size_t i = 0; i < args.size(); ++i) {
                    block.outArg(
                        session,
                        instance, callee,
                        position, i, args[i]
                    );
                }

                // render (unload the callee)

                instance.content.insert(
                    [&](OutputContext &oc) {
                        oc.endl();
                        oc.os << "inner = callee;";
                        oc.endl();
                        oc.os << "callee = inner->outer;";
                    }
                );

                // result

                after(callee, position);
            }
        );

        // render (leave)

        instance.content.insert(
            [&](OutputContext &oc) {
                oc.leave();
            }
        );
    } else {
        // TODO: object as callee
        //           return (func point to error)
        //           yield (func point to next label)
        //           reset (func point to func entry)
        throw ErrorCallNotAllowed {};
    }
}

NodeCall::NodeCall(
    Node &_source,
    std::gc_vector<std::reference_wrapper<Node>> &&_args
):
    source {_source},
    args {std::move(_args)} {}

void NodeCall::buildProc(
    Session &session,
    Instance &instance
) {
    build(
        session, instance,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_proc)) {
                throw ErrorDiscardNotAllowed {};
            }
        },
        [](Instance &, size_t) {
            // nothing
        },
        [](Instance &, size_t) {
            // nothing
        }
    );
}

Type &NodeCall::buildOut(
    Session &session,
    Instance &instance,
    std::gc_function<std::string (Type &)> &&target
) {
    Type *type_p {nullptr}; // return value

    build(
        session, instance,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_out)) {
                throw ErrorReadNotAllowed {};
            }
        },
        [](Instance &, size_t) {
            // nothing
        },
        [&](Instance &callee, size_t position) {
            Type &type {
                callee.at("result")
            };
            type_p = &type;

            // render

            instance.content.insert(
                [&, position, target = std::move(target)](OutputContext &oc) {
                    oc.endl();
                    oc.os << target(type) << " = "
                          << instance.strInner(position) << "->data.result;";
                }
            );
        }
    );

    return *type_p;
}

void NodeCall::buildIn(
    Session &session,
    Instance &instance, Type &type,
    std::gc_function<std::string (Type &)> &&target
) {
    build(
        session, instance,
        [](Block &block) {
            if (!block.getOption(BlockOption::allow_in)) {
                throw ErrorWriteNotAllowed {};
            }
        },
        [&](Instance &callee, size_t position) {
            callee.insert("input", type);

            // render

            instance.content.insert(
                [&, position, target = std::move(target)](OutputContext &oc) {
                    oc.endl();
                    oc.os << instance.strInner(position) << "->data.input = "
                          << target(type) << ";";
                }
            );
        },
        [](Instance &, size_t) {
            // nothing
        }
    );
}

}
