#pragma once

#include "include.hpp"

class Output;
class OutputContext;
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
        Output &output,
        std::function<std::string (Type &)> &&target
    ) = 0;
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::function<std::string (Type &)> &&target
    ) = 0;
};

inline bool operator==(const Node &a, const Node &b) {
    return a.nuid() == b.nuid();
}

inline bool operator!=(const Node &a, const Node &b) {
    return !(a == b);
}

inline bool operator<(const Node &a, const Node &b) {
    return a.nuid() < b.nuid();
}

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
        Output &output,
        std::function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
};
using NodeLiteralVoid = NodeLiteral<std::nullptr_t>;
using NodeLiteralBool = NodeLiteral<bool>;
using NodeLiteralInt = NodeLiteral<int64_t>;
using NodeLiteralReal = NodeLiteral<double>;
using NodeLiteralStr = NodeLiteral<std::string>;

enum class LookupMode {
    global, mixed, local
};

class NodeSymbol: public Node {
private:
    LookupMode mode;
    std::string name;

    void renderPath(std::ostream &os, size_t level) const;

public:
    NodeSymbol(LookupMode _mode, std::string &&_name);

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
};

class NodePath: public Node {
private:
    std::unique_ptr<Node> source_p;
    LookupMode mode;
    std::string name;

    void renderPath(std::ostream &os, size_t level) const;

    Instance &getInner(Instance &instance, Output &output);

public:
    NodePath(Node *_source, LookupMode _mode, std::string &&_name);

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::function<std::string (Type &)> &&target
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
    std::unique_ptr<Node> source_p;
    FrameMode mode;
    std::vector<std::unique_ptr<Node>> args;

    void renderFrameAlloc(
        Instance &instance, size_t position,
        OutputContext &oc
    ) const;
    void renderLoadCallee(OutputContext &oc) const;
    void renderBeforeCall(
        OutputContext &oc,
        const std::string &label
    ) const;
    void renderCall(
        OutputContext &oc,
        const std::string &func
    ) const;
    void renderAfterCall(
        OutputContext &oc,
        const std::string &label
    ) const;
    void renderUnloadCallee(OutputContext &oc) const;
    void renderFrameFree(
        Instance &instance, size_t position,
        OutputContext &oc
    ) const;

    void build(
        Instance &instance, Output &output,
        std::function<void (Block &)> &&init,
        std::function<void (Instance &, size_t)> &&before,
        std::function<void (Instance &, size_t)> &&after
    );

public:
    NodeCall(
        Node *_source, FrameMode _mode,
        std::vector<std::unique_ptr<Node>> &&_args
    );

    template <class... Args>
    NodeCall(
        Node *_source, FrameMode _mode,
        Args... _args
    ):
        source_p {_source},
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
        Output &output,
        std::function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
};

class NodeBlock: public Node {
private:
    std::list<std::unique_ptr<Block>> blocks;

    friend class NodeCall;

public:
    NodeBlock(std::list<std::unique_ptr<Block>> &&_blocks);

    template <class... Blocks>
    NodeBlock(Blocks... _blocks) {
        Block *init[] {_blocks...};

        for (Block *block_p: init) {
            blocks.push_back(std::unique_ptr<Block> {block_p});
        }
    }

    void addBlock(Block *block_p);

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
};
