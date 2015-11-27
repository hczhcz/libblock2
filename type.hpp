#pragma once

#include "include.hpp"

class Output;
class OutputContext;
class Instance;
class NodeBlock;
class NodeCall;
class Block;

class Type {
public:
    uintptr_t tuid() const;

    Instance &prepareLookup();

    virtual std::string strDecl(const std::string &name) const = 0;
    virtual std::string strReint(const std::string &name) const = 0;
};

inline bool operator==(const Type &a, const Type &b) {
    return a.tuid() == b.tuid();
}

inline bool operator!=(const Type &a, const Type &b) {
    return !(a == b);
}

inline bool operator<(const Type &a, const Type &b) {
    return a.tuid() < b.tuid();
}

template <class T>
class TypeNative: public Type {
private:
    TypeNative() = default;

public:
    static TypeNative<T> &get();

    virtual std::string strDecl(const std::string &name) const;
    virtual std::string strReint(const std::string &name) const;
};
using TypeNativeVoid = TypeNative<std::nullptr_t>;
using TypeNativeBool = TypeNative<bool>;
using TypeNativeInt = TypeNative<int64_t>;
using TypeNativeReal = TypeNative<double>;
using TypeNativeStr = TypeNative<std::string>;

class TypeClosure: public Type {
private:
    Type &parent;
    NodeBlock &blocks;

    friend class NodeCall;

public:
    TypeClosure(Type &_parent, NodeBlock &_blocks);

    virtual std::string strDecl(const std::string &name) const;
    virtual std::string strReint(const std::string &name) const;
};

class Instance: public Type {
private:
    static void typeCheck(Type &type1, Type &type2);

    size_t last_position {0};

    std::map<std::string, Type &> symbol_types;
    std::map<
        std::reference_wrapper<NodeBlock>,
        std::shared_ptr<TypeClosure>
    > closure_types;
    std::map<size_t, Instance &> callee_types;

public:
    Instance();

    std::string strFunc() const;
    std::string strFuncExit() const;
    std::string strStruct() const;
    std::string strSelf() const;

    std::string strLabel(size_t position) const;
    std::string strCalleeType(size_t position) const;
    std::string strCalleeName(size_t position) const;
    std::string strInner(size_t position) const;
    std::string strCallee(size_t position) const;

    void renderStruct(OutputContext &oc) const;
    void renderFuncHead(OutputContext &oc) const;
    void renderFuncTail(OutputContext &oc) const;

    bool in(Instance &instance) const;

    Type &at(const std::string &name);
    void check(const std::string &name, Type &type);
    void insert(const std::string &name, Type &type);
    Type &lookup(
        const std::string &name,
        size_t &level
    );
    void lookupCheck(
        const std::string &name, Type &type,
        size_t &level
    );

    size_t addPosition();
    void addCallee(size_t position, Instance &callee);
    Type &addClosure(Type &parent, NodeBlock &blocks);

    virtual std::string strDecl(const std::string &name) const;
    virtual std::string strReint(const std::string &name) const;
};
