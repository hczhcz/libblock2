#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>

struct Block;

//////////////// Types ////////////////

struct Type {
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

//////////////// Nodes ////////////////

struct Node {
    virtual Type &infer(Instance &instance) = 0;
    virtual void check(Instance &instance, Type &type) = 0;
};
using NodeRef = std::unique_ptr<Node>;

template <class T>
struct NodeLiteral: public Node {
    T value;

    NodeLiteral(T &&_value):
        value {std::move(_value)} {}

    virtual Type &infer(Instance &instance) {
        static TypeNative<T> type {};

        return type;
    }

    virtual void check(Instance &instance, Type &type) {
        throw std::exception {};
    }
};
using NodeLiteralBool = NodeLiteral<bool>;
using NodeLiteralInt = NodeLiteral<int64_t>;
using NodeLiteralReal = NodeLiteral<double>;
using NodeLiteralStr = NodeLiteral<std::string>;

struct NodeSymbol: public Node {
    std::string name;

    NodeSymbol(std::string &&_name):
        name {std::move(_name)} {}

    virtual Type &infer(Instance &instance) {
        const auto &symbol = instance.symbol_types.find(name);

        if (symbol != instance.symbol_types.end()) {
            return symbol->second;
        } else {
            throw std::exception {};
        }
    }

    virtual void check(Instance &instance, Type &type) {
        const auto &symbol = instance.symbol_types.find(name);

        if (symbol != instance.symbol_types.end()) {
            if (&symbol->second != &type) { // TODO
                throw std::exception {};
            }
        } else {
            instance.symbol_types.insert({name, type});
        }
    }
};

//////////////// Blocks ////////////////

struct SymbolInfo {
    bool in;
    bool out;
};

struct Block: public Node, public Type {
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

    virtual Type &infer(Instance &instance) {
        return *this;
    }

    virtual void check(Instance &instance, Type &type) {
        throw std::exception {};
    }

    Instance &getInstance(Instance &&instance) {
        // TODO
    }
};

//////////////// Calls ////////////////

struct NodeCall: public Node {
    NodeRef callee;
    std::vector<NodeRef> args;

    template <class... Args>
    NodeCall(Node *_callee, Args... _args):
        callee {_callee},
        args {} {
            Node *init[] {_args...};

            args.reserve(sizeof...(_args));
            for (Node *i: init) {
                args.push_back(NodeRef {i});
            }
        }

    virtual Type &infer(Instance &instance) {
        // TODO: special args: result, self, parent

        Type &callee_type {callee->infer(instance)};

        if (
            Block *block_p {
                dynamic_cast<Block *>(&callee_type)
            }
        ) {
            if (args.size() != block_p->params.size()) {
                // TODO: arg size != param size
                throw std::exception {};
            }

            Instance a_instance {*block_p};

            // input arguments
            for (size_t i = 0; i < args.size(); ++i) {
                std::string &param {block_p->params[i]};

                if (block_p->symbols.at(param).in) {
                    a_instance.symbol_types.insert({param, args[i]->infer(instance)});
                }
            }

            Instance &f_instance {
                block_p->getInstance(std::move(a_instance))
            };

            // output arguments
            for (size_t i = 0; i < args.size(); ++i) {
                std::string &param {block_p->params[i]};

                if (block_p->symbols.at(param).out) {
                     args[i]->check(instance, f_instance.symbol_types.at(param));
                }
            }
        } else {
            // TODO: value as callee
            throw std::exception {};
        }
    }

    virtual void check(Instance &instance, Type &type) {
        // TODO
        throw std::exception {};
    }
};

//////////////// The Builder ////////////////

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
        return new NodeSymbol {std::move(name)};
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
                    {in("a"), in("b"), tmp("t"), out("result")},
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

    return 0;
}
