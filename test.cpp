#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>

struct Instance;
struct NodeBlock;

struct Typed {
    NodeBlock &block;
    Instance &instance;
};

template <class T>
struct TypedNode: public Typed, public T {};

struct TypedSymbol: public Typed {};

struct Node {
    virtual std::unique_ptr<Typed> build(NodeBlock &block, Instance &instance) {
        // TODO
    }
};

template <class T>
struct NodeLiteral: public Node {
    T value;

    NodeLiteral(T &&_value):
        value {std::move(_value)} {}
};
struct NodeLiteralBool: public NodeLiteral<bool> {
    using NodeLiteral::NodeLiteral;
};
struct NodeLiteralInt: public NodeLiteral<int64_t> {
    using NodeLiteral::NodeLiteral;
};
struct NodeLiteralReal: public NodeLiteral<double> {
    using NodeLiteral::NodeLiteral;
};
struct NodeLiteralStr: public NodeLiteral<std::string> {
    using NodeLiteral::NodeLiteral;
};
struct NodeLiteralSymbol: public NodeLiteral<std::string> {
    using NodeLiteral::NodeLiteral;
};

struct NodeCall: public Node {
    std::unique_ptr<Node> callee;
    std::vector<Node> args;

    NodeCall(Node &&_callee, decltype(args) &&_args):
        callee {new Node {std::move(_callee)}},
        args {std::move(_args)} {}
};

struct Symbol {
    bool in;
    bool out;
};

struct Instance {
    std::map<std::string, TypedSymbol> symbols;
    std::unique_ptr<Node> ast;
};

struct NodeBlock: public Node {
    std::vector<std::string> args;
    std::map<std::string, Symbol> symbols;
    std::unique_ptr<Node> ast;

    std::vector<Instance> instances;

    NodeBlock(decltype(args) &&_args, decltype(symbols) &&_symbols, Node &&_ast):
        args {std::move(_args)},
        symbols {std::move(_symbols)},
        ast {new Node {std::move(_ast)}} {}
};

namespace builder {
    NodeLiteralBool _(bool &&value) {
        return {std::move(value)};
    }

    NodeLiteralInt _(int64_t &&value) {
        return {std::move(value)};
    }

    NodeLiteralReal _(double &&value) {
        return {std::move(value)};
    }

    NodeLiteralStr _(std::string &&value) {
        return {std::move(value)};
    }

    NodeLiteralSymbol $(std::string &&value) {
        return {std::move(value)};
    }

    using block = NodeBlock;
    using call = NodeCall;

    template <class T>
    NodeCall $(T &&callee, decltype(NodeCall::args) &&args) {
        return {$(std::move(callee)), std::move(args)};
    }

    using symbol_pair_t = decltype(NodeBlock::symbols)::value_type;

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

    NodeBlock root {
        {},
        {tmp("c")},
        $(";", {
            $("=", {
                $("c"),
                NodeBlock {
                    {"a", "b"},
                    {in("a"), in("b"), tmp("t"), out("r")},
                    $(";", {
                        $("=", {
                            $("t"), $("*", {_(2l), $("b")}),
                        }),
                        $("=", {
                            $("r"), $("+", {$("a"), $("t")}),
                        }),
                    })
                }
            }),
            $("c", {
                _("xx"), _("yy")
            })
        })
    };

    return 0;
}
