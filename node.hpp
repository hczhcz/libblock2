#pragma once

#include "include.hpp"

namespace libblock {

class Output;
class OutputContext;
class Type;
class TypeClosure;
class Instance;
class Block;

class Node {
protected:
    Node();

    Node(Node &&) = delete;
    Node(const Node &) = delete;

public:
    virtual void buildProc(
        Instance &instance,
        Output &output
    ) = 0;
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    ) = 0;
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    ) = 0;
};

template <class T>
class NodeLiteral: public Node {
private:
    T value;

    void renderValue(std::ostream &os) const;

public:
    NodeLiteral(T &&_value);
    NodeLiteral(const T &_value);

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
};
using NodeLiteralVoid = NodeLiteral<lb_void_t>;
using NodeLiteralBool = NodeLiteral<lb_bool_t>;
using NodeLiteralInt = NodeLiteral<lb_int_t>;
using NodeLiteralReal = NodeLiteral<lb_real_t>;
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
    NodeSymbol(LookupMode _mode, const std::string &_name);

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
};

class NodePath: public Node {
private:
    Node &source;
    LookupMode mode;
    std::string name;

    void renderPath(std::ostream &os, size_t level) const;

    Instance &getInner(Instance &instance, Output &output);

public:
    NodePath(Node &_source, LookupMode _mode, std::string &&_name);
    NodePath(Node &_source, LookupMode _mode, const std::string &_name);

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
};

class NodeCall: public Node {
private:
    Node &source;
    std::gc_vector<std::reference_wrapper<Node>> args;

    void renderLabelDef(
        OutputContext &och,
        const std::string &label
    ) const;
    void renderLoadCallee(OutputContext &oc) const;
    void renderCall(
        OutputContext &oc,
        const std::string &label,
        const std::string &func
    ) const;
    void renderUnloadCallee(OutputContext &oc) const;

    void build(
        Instance &instance, Output &output,
        std::gc_function<void (Block &)> &&init,
        std::gc_function<void (Instance &, size_t)> &&before,
        std::gc_function<void (Instance &, size_t)> &&after
    );

public:
    NodeCall(
        Node &_source,
        std::gc_vector<std::reference_wrapper<Node>> &&_args
    );

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
};

class NodeBlock: public Node {
private:
    Node &source;
    std::gc_vector<std::reference_wrapper<Block>> blocks;

    friend class NodeCall;

public:
    NodeBlock(
        Node &_source,
        std::gc_vector<std::reference_wrapper<Block>> &&_blocks
    );

    void addBlock(Block &block_p);

    virtual void buildProc(
        Instance &instance,
        Output &output
    );
    virtual Type &buildOut(
        Instance &instance,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
    virtual void buildIn(
        Instance &instance, Type &type,
        Output &output,
        std::gc_function<std::string (Type &)> &&target
    );
};

}
