#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>

struct Instance;
struct NodeBlock;

struct Node;
using NodeRef = std::unique_ptr<Node>;
struct Typed;
using TypedRef = std::unique_ptr<Typed>;

struct Typed {
    NodeBlock &block;
    Instance &instance;
};

template <class T>
struct TypedNode: public Typed, public T {};

struct TypedSymbol: public Typed {};

struct Node {
    virtual Typed *build(NodeBlock &block, Instance &instance) {
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
    NodeRef callee;
    std::vector<NodeRef> args;

    template <class... Args>
    NodeCall(Node *_callee, Args... _args):
        callee {_callee} {} // TODO: push?
};

struct Symbol {
    bool in;
    bool out;
};

struct Instance {
    std::map<std::string, TypedSymbol> symbols;
    NodeRef ast;
};

struct NodeBlock: public Node {
    std::vector<std::string> args;
    std::map<std::string, Symbol> symbols;
    NodeRef ast;

    std::vector<Instance> instances;

    NodeBlock(decltype(args) &&_args, decltype(symbols) &&_symbols, Node *_ast):
        args {std::move(_args)},
        symbols {std::move(_symbols)},
        ast {_ast} {}
};

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

    NodeLiteralSymbol *$(std::string &&value) {
        return new NodeLiteralSymbol {std::move(value)};
    }

    template <class... Args>
    NodeCall *call(Node *callee, Args... args) {
        return new NodeCall {callee, args...};
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
        call(
            $(";"),
            call(
                $("="), $("c"), new NodeBlock {
                    {"a", "b"},
                    {in("a"), in("b"), tmp("t"), out("r")},
                    call(
                        $(";"),
                        call(
                            $("="), $("t"), call($("*"), _(2), $("b"))
                        ),
                        call(
                            $("="), $("r"), call($("+"), $("a"), $("t"))
                        )
                    )
                }
            ),
            call($("c"), _("xx"), _("yy"))
        )
    };

    return 0;
}
