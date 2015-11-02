#pragma once

#include <string>
#include <memory>
#include <vector>

struct Output;
struct Type;
struct Instance;

struct Node {
    inline uintptr_t nuid() const {
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

    inline NodeLiteral(T &&_value):
        value {std::move(_value)} {}

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
