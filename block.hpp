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

    inline Type &lookup(const std::string &name) {
        const auto &symbol = symbol_types.find(name);

        if (symbol != symbol_types.end()) {
            return symbol->second;
        } else {
            throw std::exception {};
        }
    }

    template <class Callback, class Fail>
    inline void lookup(const std::string &name, Callback &&callback, Fail &&fail) {
        const auto &symbol = symbol_types.find(name);

        if (symbol != symbol_types.end()) {
            callback(symbol->second);
        } else {
            fail();
        }
    }

    // as type
    virtual std::string typeName();
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
    virtual std::string typeName();
};
