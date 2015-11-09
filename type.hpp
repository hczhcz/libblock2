#pragma once

#include "include.hpp"

class Output;
class OutputContext;
class Instance;
class Node;
class Block;

class Type {
protected:
    uintptr_t tuid() const;

    friend inline bool operator==(const Type &a, const Type &b) {
        return a.tuid() == b.tuid();
    }

    friend inline bool operator!=(const Type &a, const Type &b) {
        return !(a == b);
    }

public:
    virtual std::string strDecl(const std::string &name) const = 0;
};

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
        std::function<void (Instance &, Block &)> &&before,
        std::function<void (Instance &, Block &)> &&after
    );

    virtual std::string strDecl(const std::string &name) const;
};

class Instance: public Type {
private:
    std::map<std::string, Type &> symbol_types;

    friend class Block;
    friend class Output;

public:
    Type &at(const std::string &name);
    void insert(const std::string &name, Type &type);
    Instance &lookup(const std::vector<std::string> &path);
    Type &fullLookup(
        const std::vector<std::string> &path,
        const std::string &name,
        size_t &level
    ); // also lookup parent->path->name

    std::string strFunc() const;
    std::string strStruct() const;
    std::string strCast() const;

    void renderStruct(OutputContext &oc) const;
    void renderFuncHead(OutputContext &oc) const;
    void renderFuncTail(OutputContext &oc) const;

    virtual std::string strDecl(const std::string &name) const;
};
