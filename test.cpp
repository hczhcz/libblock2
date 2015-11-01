#include "util.hpp"

#include <sstream>
#include <iostream>

struct Block;

//////////////// Types ////////////////

struct Type {
    uintptr_t tuid() const {
        return (uintptr_t) this;
    }

    virtual void print() {
        // TODO
    }
};

template <class T>
struct TypeNative: public Type {
    //
};

struct Instance: public Type {
    Block &block;

    std::map<std::string, Type &> symbol_types;

    Instance(Block &_block):
        block {_block} {}
};

bool operator==(const Type &a, const Type &b) {
    return a.tuid() == b.tuid();
}

bool operator!=(const Type &a, const Type &b) {
    return !(a == b);
}

//////////////// Output ////////////////

struct OutputContext {
    std::ostringstream header;
    std::ostringstream content;
};

using Output = std::map<uintptr_t, OutputContext>;

//////////////// Nodes ////////////////

struct Node {
    uintptr_t nuid() const {
        return (uintptr_t) this;
    }

    virtual void buildProc(Instance &instance, Output &output) = 0;
    virtual Type &buildOut(Instance &instance, Output &output) = 0;
    virtual void buildIn(Instance &instance, Type &type, Output &output) = 0;
};
using NodeRef = std::unique_ptr<Node>;

template <class T>
struct NodeLiteral: public Node {
    T value;

    NodeLiteral(T &&_value):
        value {std::move(_value)} {}

    virtual void buildProc(Instance &instance, Output &output) {
        // nothing
    }

    virtual Type &buildOut(Instance &instance, Output &output) {
        // render

        output.at(instance.tuid()).content
            << "    " << util::cType("result_" + std::to_string(nuid()), value)
            << " = " << util::cCode(value) << ";\n";

        // get type

        static TypeNative<T> type {};
        return type;
    }

    virtual void buildIn(Instance &instance, Type &type, Output &output) {
        throw std::exception {};
    }
};
using NodeLiteralBool = NodeLiteral<bool>;
using NodeLiteralInt = NodeLiteral<int64_t>;
using NodeLiteralReal = NodeLiteral<double>;
using NodeLiteralStr = NodeLiteral<std::string>;

struct NodeSymbol: public Node {
    std::vector<std::string> path;

    NodeSymbol(std::vector<std::string> &&_path):
        path {std::move(_path)} {}

    virtual void buildProc(Instance &instance, Output &output) {
        // TODO: lookup?
        // TODO: render

        const auto &symbol = instance.symbol_types.find(path[0]); // TODO

        if (symbol != instance.symbol_types.end()) {
            // nothing
        } else {
            throw std::exception {};
        }
    }

    virtual Type &buildOut(Instance &instance, Output &output) {
        // TODO: lookup?
        // TODO: render

        const auto &symbol = instance.symbol_types.find(path[0]); // TODO

        if (symbol != instance.symbol_types.end()) {
            return symbol->second;
        } else {
            throw std::exception {};
        }
    }

    virtual void buildIn(Instance &instance, Type &type, Output &output) {
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
};

//////////////// Blocks ////////////////

struct SymbolInfo {
    bool in;
    bool out;
};

struct Block: public Node, public Type {
    // TODO: multiple signature (overloading and SFINAE)
    std::vector<std::string> params;
    std::map<std::string, SymbolInfo> symbols;
    NodeRef ast;

    std::vector<Instance> instances;

    Block(
        std::vector<std::string> &&_params,
        std::map<std::string, SymbolInfo> &&_symbols,
        Node *_ast
    ):
        params {std::move(_params)},
        symbols {std::move(_symbols)},
        ast {_ast} {}

    Instance &getInstance(Instance &&instance, Output &output) {
        for (Instance &target: instances) {
            bool ok {true};

            for (const auto &symbol: instance.symbol_types) {
                if (symbol.second != target.symbol_types.at(symbol.first)) {
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

        // TODO: render

        instances.push_back(std::move(instance));
        ast->buildProc(instances.back(), output);

        return instances.back();
    }

    virtual void buildProc(Instance &instance, Output &output) {
        // nothing
    }

    virtual Type &buildOut(Instance &instance, Output &output) {
        // render

        uintptr_t id {tuid()};

        output.at(instance.tuid()).content
            << "    " << util::cType("result_" + std::to_string(nuid()), id)
            << " = " << util::cCode(id) << ";\n";

        // get type

        return *this;
    }

    virtual void buildIn(Instance &instance, Type &type, Output &output) {
        throw std::exception {};
    }
};

//////////////// Calls ////////////////

struct NodeCall: public Node {
    NodeRef callee;
    std::vector<NodeRef> args;

    template <class... Args>
    NodeCall(Node *_callee, Args... _args):
        callee {_callee} {
            Node *init[] {_args...};

            args.reserve(sizeof...(_args));
            for (Node *i: init) {
                args.push_back(NodeRef {i});
            }
        }

    template <class Before, class After>
    void build(Instance &instance, Output &output, Before before, After after) {
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

    virtual void buildProc(Instance &instance, Output &output) {
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

    virtual Type &buildOut(Instance &instance, Output &output) {
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

    virtual void buildIn(Instance &instance, Type &type, Output &output) {
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
};

//////////////// Node Tree Builder ////////////////

namespace builder {
    NodeLiteralBool *_(bool &&value) {
        return new NodeLiteralBool {std::move(value)};
    }

    NodeLiteralInt *_(int32_t &&value) {
        return new NodeLiteralInt {std::move(value)};
    }

    NodeLiteralInt *_(int64_t &&value) {
        return new NodeLiteralInt {std::move(value)};
    }

    NodeLiteralReal *_(double &&value) {
        return new NodeLiteralReal {std::move(value)};
    }

    NodeLiteralStr *_(std::string &&value) {
        return new NodeLiteralStr {std::move(value)};
    }

    NodeSymbol *$(std::string &&name) {
        return new NodeSymbol {{std::move(name)}};
    }

    NodeSymbol *$(std::vector<std::string> &&path) {
        return new NodeSymbol {std::move(path)};
    }

    template <class... Args>
    NodeCall *call(Node *callee, Args... args) {
        return new NodeCall {callee, args...};
    }

    Block *block(
        std::vector<std::string> &&params,
        std::map<std::string, SymbolInfo> &&symbols,
        Node *ast
    ) {
        return new Block {std::move(params), std::move(symbols), ast};
    }

    using symbol_pair_t = std::pair<std::string, SymbolInfo>;

    symbol_pair_t var(std::string &&name) {
        return {std::move(name), {true, true}};
    }

    symbol_pair_t in(std::string &&name) {
        return {std::move(name), {true, false}};
    }

    symbol_pair_t out(std::string &&name) {
        return {std::move(name), {false, true}};
    }

    symbol_pair_t tmp(std::string &&name) {
        return {std::move(name), {false, false}};
    }
}

int main() {
    using namespace builder;

    Block root {
        {},
        {tmp("c")},
        call(
            $(";"),
            call(
                $("="), $("c"), block(
                    {"a", "b"},
                    {in("a"), in("b"), tmp("t")},
                    call(
                        $(";"),
                        call(
                            $("="), $("t"), call($("*"), _(2), $("b"))
                        ),
                        call(
                            $("="), $("result"), call($("+"), $("a"), $("t"))
                        )
                    )
                )
            ),
            call($("c"), _("xx"), _("yy"))
        )
    };

    Output output;

    root.getInstance(Instance {root}, output);

    return 0;
}
