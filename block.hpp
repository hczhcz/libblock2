#pragma once

#include "type.hpp"
#include "node.hpp"

#include <string>
#include <vector>
#include <map>

struct SymbolInfo {
    bool in;
    bool out;
};

struct Instance: public Type {
    Block &block;

    std::map<std::string, Type &> symbol_types;

    inline Instance(Block &_block):
        block {_block} {}

    virtual std::string typeName();
};

struct Block: public Node, public Type {
    // TODO: multiple signature (overloading and SFINAE)
    std::vector<std::string> params;
    std::map<std::string, SymbolInfo> symbols;
    NodeRef ast;

    std::vector<Instance> instances;

    inline Block(
        std::vector<std::string> &&_params,
        std::map<std::string, SymbolInfo> &&_symbols,
        Node *_ast
    ):
        params {std::move(_params)},
        symbols {std::move(_symbols)},
        ast {_ast} {}

    Instance &getInstance(Instance &&instance, Output &output);

    virtual void buildProc(Instance &instance, Output &output);
    virtual Type &buildOut(Instance &instance, Output &output);
    virtual void buildIn(Instance &instance, Type &type, Output &output);

    virtual std::string typeName();
};
