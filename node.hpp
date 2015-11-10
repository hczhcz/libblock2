#pragma once

#include "include.hpp"

class Output;
class Type;
class TypeClosure;
class Instance;
class Block;

class Node {
public:
    uintptr_t nuid() const;

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
    NodeLiteral(T &&_value);

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
    std::string name;

    void renderPath(std::ostream &os, size_t level) const;

public:
    NodeSymbol(std::string &&_name);

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

class NodePath: public Node {
private:
    std::unique_ptr<Node> source_p;
    std::string name;

    void renderPath(std::ostream &os) const;

public:
    NodePath(Node *_source, std::string &&_name);

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

enum class FrameMode {
    static_global,
    static_local,
    dynamic_stack,
    dynamic_gc,
    dynamic_free
};

class NodeCall: public Node {
private:
    std::unique_ptr<Node> callee_p;
    FrameMode mode;
    std::vector<std::unique_ptr<Node>> args;

    std::string strFrame(Instance &instance) const;
    std::string strObject(Instance &instance) const;
    std::string strLabel(Instance &instance) const;
    std::string strInner(Instance &instance) const;
    std::string strCallee(Instance &instance) const;

    void build(
        Instance &instance, Output &output,
        std::function<void (Instance &)> &&before,
        std::function<void (Instance &)> &&after
    );

public:
    template <class... Args>
    NodeCall(Node *_callee, FrameMode _mode, Args... _args):
        callee_p {_callee},
        mode {_mode} {
            Node *init[] {_args...};

            args.reserve(sizeof...(_args));
            for (Node *arg_p: init) {
                args.push_back(std::unique_ptr<Node> {arg_p});
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

class NodeBlock: public Node {
private:
    std::vector<std::unique_ptr<Block>> blocks;

    friend class TypeClosure;

public:
    template <class... Blocks>
    NodeBlock(Blocks... _blocks) {
        Block *init[] {_blocks...};

        blocks.reserve(sizeof...(_blocks));
        for (Block *block_p: init) {
            blocks.push_back(std::unique_ptr<Block> {block_p});
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
