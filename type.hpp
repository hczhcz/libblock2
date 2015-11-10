#pragma once

#include "include.hpp"

class Output;
class OutputContext;
class Instance;
class Node;
class Block;

class Type {
public:
    uintptr_t tuid() const;

    Instance &prepareLookup();

    virtual std::string strDecl(const std::string &name) const = 0;
};

inline bool operator==(const Type &a, const Type &b) {
    return a.tuid() == b.tuid();
}

inline bool operator!=(const Type &a, const Type &b) {
    return !(a == b);
}

template <class T>
class TypeNative: public Type {
public:
    virtual std::string strDecl(const std::string &name) const;
};

class TypeClosure: public Type {
private:
    Instance &parent;
    Block &block;

public:
    inline TypeClosure(Instance &_parent, Block &_block):
        parent {_parent},
        block {_block} {}

    void call(
        Output &output,
        std::function<void (Instance &, Block &, Instance &)> &&before,
        std::function<void (Instance &, Block &)> &&after
    );

    virtual std::string strDecl(const std::string &name) const;
};

class Instance: public Type {
private:
    std::map<std::string, Type &> symbol_types;

    friend class Block;

public:
    std::string strFunc() const;
    std::string strStruct() const;
    std::string strCast(const std::string &name) const;

    void renderStruct(OutputContext &oc) const;
    void renderFuncHead(OutputContext &oc) const;
    void renderFuncTail(OutputContext &oc) const;

    Type &at(const std::string &name);
    void insert(const std::string &name, Type &type);
    Type &lookup(const std::string &name, size_t &level);

    virtual std::string strDecl(const std::string &name) const;
};
