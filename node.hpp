#pragma once

#include "type.hpp"

#include <memory>
#include <sstream>

struct Output;

struct Node {
    uintptr_t nuid() const;
    std::string nuidOut() const;
    std::string nuidIn() const;

    virtual void buildProc(Instance &instance, Output &output) = 0;
    virtual Type &buildOut(Instance &instance, Output &output) = 0;
    virtual void buildIn(Instance &instance, Type &type, Output &output) = 0;
};
using NodeRef = std::unique_ptr<Node>;

template <class T>
struct NodeLiteral: public Node {
    T value;

    inline NodeLiteral(T &&_value):
        value {std::move(_value)} {}

    std::string renderValue();

    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);
};
using NodeLiteralBool = NodeLiteral<bool>;
using NodeLiteralInt = NodeLiteral<int64_t>;
using NodeLiteralReal = NodeLiteral<double>;
using NodeLiteralStr = NodeLiteral<std::string>;

struct NodeSymbol: public Node {
    std::vector<std::string> path;
    std::string name;

    inline NodeSymbol(std::vector<std::string> &&_path):
        path {std::move(_path)},
        name {path.back()} {
            path.pop_back();
        }

    void renderPath(std::ostringstream &os);

    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);
};

struct NodeCall: public Node {
    NodeRef callee;
    std::vector<NodeRef> args;

    template <class... Args>
    inline NodeCall(Node *_callee, Args... _args):
        callee {_callee} {
            Node *init[] {_args...};

            args.reserve(sizeof...(_args));
            for (Node *i: init) {
                args.push_back(NodeRef {i});
            }
        }

    template <class Before, class After>
    void build(Instance &instance, Output &output, Before &&before, After &&after);

    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);
};

enum class SymbolMode {
    in, out, var
};

struct Block: public Node {
    // TODO: multiple signature (overloading and SFINAE)
    std::vector<std::pair<std::string, SymbolMode>> params;

    std::vector<Instance> instances;

    inline Block(std::vector<std::pair<std::string, SymbolMode>> &&_params):
        params {std::move(_params)} {}

    Instance &getInstance(Instance &&instance, Output &output);

    virtual void buildContent(Instance &instance, Output &output) = 0;

    // as node
    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);
};

struct BlockBuiltin: public Block {
    std::string name;

    inline BlockBuiltin(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        std::string &&_name
    ):
        Block {std::move(_params)},
        name {_name} {}

    // as block
    virtual void buildContent(Instance &instance, Output &output);
};

struct BlockUser: public Block {
    NodeRef ast;

    inline BlockUser(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        Node *_ast
    ):
        Block {std::move(_params)},
        ast {_ast} {}

    // as block
    virtual void buildContent(Instance &instance, Output &output);
};
