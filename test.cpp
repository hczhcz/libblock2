#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>

struct Instance;
struct NodeBlock;

struct Instance {
    NodeBlock &block;

    std::map<std::string, Instance &> symbol_types;
};

struct Type {
    //
};

struct TypeBlock: public Type, public Instance {
    //
};

struct Node;
using NodeRef = std::unique_ptr<Node>;

struct Node {
    virtual void infer(Instance &instance) = 0;
};

template <class T>
struct NodeLiteral: public Node {
    T value;

    NodeLiteral(T &&_value):
        value {std::move(_value)} {}

    virtual void infer(Instance &instance) {
        // TODO
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

    virtual void infer(Instance &instance) {
        // TODO
    }
};

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

    virtual void infer(Instance &instance) {
        // TODO
    }
};

struct SymbolInfo {
    bool in;
    bool out;
};

struct NodeBlock: public Node {
    std::vector<std::string> args;
    std::map<std::string, SymbolInfo> symbols;
    NodeRef ast;

    std::vector<Instance> instances;

    NodeBlock(
        std::vector<std::string> &&_args,
        std::map<std::string, SymbolInfo> &&_symbols,
        Node *_ast
    ):
        args {std::move(_args)},
        symbols {std::move(_symbols)},
        ast {_ast} {}

    virtual void infer(Instance &instance) {
        // TODO
    }
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

    NodeSymbol *$(std::string &&name) {
        return new NodeSymbol {std::move(name)};
    }

    template <class... Args>
    NodeCall *call(Node *callee, Args... args) {
        return new NodeCall {callee, args...};
    }

    NodeBlock *block(
        std::vector<std::string> &&args,
        std::map<std::string, SymbolInfo> &&symbols,
        Node *ast
    ) {
        return new NodeBlock {std::move(args), std::move(symbols), ast};
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

    NodeBlock root {
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
