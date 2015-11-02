#pragma once

#include "type.hpp"
#include "node.hpp"

enum class SymbolMode {
    in, out, var
};

struct Instance: public Type {
    Block &block;

    std::map<std::string, Type &> symbol_types;

    inline Instance(Block &_block):
        block {_block} {}

    Type &at(const std::string &name);
    void insert(const std::string &name, Type &type);
    Instance &lookup(const std::vector<std::string> &path);

    // as type
    virtual std::string renderDecl(std::string &&name) const;
};

struct Block: public Node, public Type {
    // TODO: multiple signature (overloading and SFINAE)
    std::vector<std::pair<std::string, SymbolMode>> params;
    NodeRef ast;

    std::vector<Instance> instances;

    inline Block(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        Node *_ast
    ):
        params {std::move(_params)},
        ast {_ast} {}

    Instance &getInstance(Instance &&instance, Output &output);

    // as node
    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);

    // as type
    virtual std::string renderDecl(std::string &&name) const;
};
