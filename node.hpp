#pragma once

#include "type.hpp"

class Node {
protected:
    uintptr_t nuid() const;

public:
    virtual void buildProc(
        Instance &instance,
        Output &output
    ) = 0;
    virtual Type &buildOut(
        Instance &instance,
        Output &output, const std::string &target
    ) = 0;
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output, const std::string &target
    ) = 0;
};

template <class T>
class NodeLiteral: public Node {
private:
    T value;

    void renderValue(std::ostream &os) const;

public:
    inline NodeLiteral(T &&_value):
        value {std::move(_value)} {}

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output, const std::string &target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output, const std::string &target
    );
};
using NodeLiteralBool = NodeLiteral<bool>;
using NodeLiteralInt = NodeLiteral<int64_t>;
using NodeLiteralReal = NodeLiteral<double>;
using NodeLiteralStr = NodeLiteral<std::string>;

class NodeSymbol: public Node {
private:
    std::vector<std::string> path;
    std::string name;

    size_t level {0};

    void renderPath(std::ostream &os) const;

public:
    inline NodeSymbol(std::vector<std::string> &&_path):
        path {std::move(_path)},
        name {path.back()} {
            path.pop_back();
        }

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output, const std::string &target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output, const std::string &target
    );
};

class NodeCall: public Node {
private:
    std::unique_ptr<Node> callee;
    std::vector<std::unique_ptr<Node>> args;

    std::string strFrame() const;
    void build(
        Instance &instance, Output &output,
        std::function<void (Instance &)> &&before,
        std::function<void (Instance &)> &&after
    );

public:
    template <class... Args>
    inline NodeCall(Node *_callee, Args... _args):
        callee {_callee} {
            Node *init[] {_args...};

            args.reserve(sizeof...(_args));
            for (Node *arg: init) {
                args.push_back(std::unique_ptr<Node> {arg});
            }
        }

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output, const std::string &target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output, const std::string &target
    );
};

enum class SymbolMode {
    in, out, var, special
};

class Block: public Node {
private:
    // TODO: multiple signature (overloading and SFINAE)
    std::vector<std::pair<std::string, SymbolMode>> params;

    std::map<uintptr_t, std::shared_ptr<TypeClosure>> closure_types;
    std::vector<std::unique_ptr<Instance>> instances;

protected:
    Instance &matchInstance(
        std::unique_ptr<Instance> &&instance,
        Output &output
    );

    virtual void inSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );
    virtual void buildContent(Instance &instance, Output &output) = 0;
    virtual void outSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );

public:
    inline Block(std::vector<std::pair<std::string, SymbolMode>> &&_params):
        params {std::move(_params)} {}

    void inArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );
    void outArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );
    void build(
        Output &output,
        std::function<void (Instance &)> &&before,
        std::function<void (Instance &)> &&after
    );

    Type &addClosureType(Instance &instance);

    // as node
    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output, const std::string &target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output, const std::string &target
    );
};

class BlockBuiltin: public Block {
private:
    std::string name;

    static std::map<std::string, BlockBuiltin &> &builtins();

public:
    inline BlockBuiltin(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        std::string &&_name
    ):
        Block {std::move(_params)},
        name {std::move(_name)} {
            builtins().insert({name, *this});
        }

    static void applyBuiltin(Instance &instance);
};

class BlockUser: public Block {
private:
    std::unique_ptr<Node> ast;

protected:
    // as block
    virtual void buildContent(Instance &instance, Output &output);

public:
    inline BlockUser(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        Node *_ast
    ):
        Block {std::move(_params)},
        ast {_ast} {}
};
