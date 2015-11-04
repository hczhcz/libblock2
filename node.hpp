#pragma once

#include "type.hpp"

#ifndef NO_STD_LIB
#include <memory>
#endif

struct Output;

struct Node {
    uintptr_t nuid() const;
    std::string nuidOut() const;
    std::string nuidIn() const;

    virtual void buildProc(Instance &instance, Output &output) = 0;
    virtual Type &buildOut(Instance &instance, Output &output) = 0;
    virtual void buildIn(Instance &instance, Type &type, Output &output) = 0;
};

template <class T>
struct NodeLiteral: public Node {
    T value;

    inline NodeLiteral(T &&_value):
        value {std::move(_value)} {}

    void renderValue(std::ostringstream &os) const;

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

    size_t level {0};

    inline NodeSymbol(std::vector<std::string> &&_path):
        path {std::move(_path)},
        name {path.back()} {
            path.pop_back();
        }

    void renderPath(std::ostringstream &os) const;

    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);
};

struct NodeCall: public Node {
    std::unique_ptr<Node> callee;
    std::vector<std::unique_ptr<Node>> args;

    template <class... Args>
    inline NodeCall(Node *_callee, Args... _args):
        callee {_callee} {
            Node *init[] {_args...};

            args.reserve(sizeof...(_args));
            for (Node *i: init) {
                args.push_back(std::unique_ptr<Node> {i});
            }
        }

    template <class Before, class After>
    void build(
        Instance &instance, Output &output,
        Before &&before, After &&after
    );

    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);
};

enum class SymbolMode {
    in, out, var, special
};

struct Block: public Node {
    // TODO: multiple signature (overloading and SFINAE)
    std::vector<std::pair<std::string, SymbolMode>> params;

    std::vector<std::unique_ptr<Instance>> instances;

    inline Block(std::vector<std::pair<std::string, SymbolMode>> &&_params):
        params {std::move(_params)} {}

    std::unique_ptr<Instance> initInstance(Instance &parent);
    void inArg(
        Instance &parent, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output
    );
    Instance &matchInstance(
        std::unique_ptr<Instance> &&instance,
        Output &output
    );
    void outArg(
        Instance &parent, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output
    );

    virtual void inSpecialArg(
        Instance &parent, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output
    );
    virtual void buildContent(Instance &instance, Output &output) = 0;
    virtual void outSpecialArg(
        Instance &parent, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output
    );

    // as node
    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);
};

struct BlockBuiltin: public Block {
    static std::map<std::string, BlockBuiltin &> &builtins();

    static void applyBuiltin(Instance &instance);

    std::string name;

    inline BlockBuiltin(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        std::string &&_name
    ):
        Block {std::move(_params)},
        name {std::move(_name)} {
            builtins().insert({name, *this});
        }

    // as block
    virtual void buildContent(Instance &instance, Output &output);
};

struct BlockUser: public Block {
    std::unique_ptr<Node> ast;

    inline BlockUser(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        Node *_ast
    ):
        Block {std::move(_params)},
        ast {_ast} {}

    // as block
    virtual void buildContent(Instance &instance, Output &output);
};
