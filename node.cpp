#include "util.hpp"
#include "type.hpp"
#include "output.hpp"
#include "node.hpp"
#include "block.hpp"

template <class T>
void NodeLiteral<T>::buildProc(Instance &instance, Output &output) {
    // nothing
}

template <class T>
Type &NodeLiteral<T>::buildOut(Instance &instance, Output &output) {
    // render

    output.at(instance.tuid()).content
        << "    " << util::cType("result_" + std::to_string(nuid()), value)
        << " = " << util::cCode(value) << ";\n";

    // get type

    static TypeNative<T> type {};
    return type;
}

template <class T>
void NodeLiteral<T>::buildIn(Instance &instance, Type &type, Output &output) {
    throw std::exception {};
}

template class NodeLiteral<bool>;
template class NodeLiteral<int64_t>;
template class NodeLiteral<double>;
template class NodeLiteral<std::string>;

void NodeSymbol::buildProc(Instance &instance, Output &output) {
    // TODO: lookup?
    // TODO: render

    const auto &symbol = instance.symbol_types.find(path[0]); // TODO

    if (symbol != instance.symbol_types.end()) {
        // nothing
    } else {
        throw std::exception {};
    }
}

Type &NodeSymbol::buildOut(Instance &instance, Output &output) {
    // TODO: lookup?
    // TODO: render

    const auto &symbol = instance.symbol_types.find(path[0]); // TODO

    if (symbol != instance.symbol_types.end()) {
        return symbol->second;
    } else {
        throw std::exception {};
    }
}

void NodeSymbol::buildIn(Instance &instance, Type &type, Output &output) {
    // TODO: lookup?
    // TODO: render

    const auto &symbol = instance.symbol_types.find(path[0]); // TODO

    if (symbol != instance.symbol_types.end()) {
        if (symbol->second != type) {
            throw std::exception {};
        }
    } else {
        instance.symbol_types.insert({path[0], type}); // TODO
    }
}

template <class Before, class After>
void NodeCall::build(Instance &instance, Output &output, Before before, After after) {
    // TODO: special args: input, result, self, parent
    // TODO: render

    Type &callee_type {callee->buildOut(instance, output)};

    if (
        Block *block_p {
            dynamic_cast<Block *>(&callee_type)
        }
    ) {
        if (args.size() != block_p->params.size()) {
            // error: arg size != param size
            throw std::exception {};
        }

        Instance a_instance {*block_p};

        before(a_instance);

        // input arguments
        for (size_t i = 0; i < args.size(); ++i) {
            std::string &param {block_p->params[i]};

            if (block_p->symbols.at(param).in) {
                a_instance.symbol_types.insert({
                    param, args[i]->buildOut(instance, output)
                });
            }
        }

        Instance &f_instance {
            block_p->getInstance(std::move(a_instance), output)
        };

        // output arguments
        for (size_t i = 0; i < args.size(); ++i) {
            std::string &param {block_p->params[i]};

            if (block_p->symbols.at(param).out) {
                const auto &symbol = f_instance.symbol_types.find(param);

                if (symbol != f_instance.symbol_types.end()) {
                    args[i]->buildIn(
                        instance, symbol->second, output
                    );
                } else {
                    throw std::exception {};
                }
            }
        }

        after(f_instance);
    } else {
        // error: value as callee
        throw std::exception {};
    }
}

void NodeCall::buildProc(Instance &instance, Output &output) {
    build(
        instance, output,
        [](Instance &instance) {
            // nothing
        },
        [](Instance &instance) {
            // nothing
        }
    );
}

Type &NodeCall::buildOut(Instance &instance, Output &output) {
    Type *type_p;

    build(
        instance, output,
        [](Instance &instance) {
            // nothing
        },
        [&](Instance &instance) {
            const auto &symbol = instance.symbol_types.find("result");

            if (symbol != instance.symbol_types.end()) {
                // TODO: instance.block.symbols.at("result").out ?
                type_p = &symbol->second;
            } else {
                throw std::exception {};
            }
        }
    );

    return *type_p;
}

void NodeCall::buildIn(Instance &instance, Type &type, Output &output) {
    build(
        instance, output,
        [&](Instance &instance) {
            // TODO: instance.block.symbols.at("input").in ?
            instance.symbol_types.insert({"input", type});
        },
        [](Instance &instance) {
            // nothing
        }
    );
}
