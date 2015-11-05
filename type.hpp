#pragma once

#ifndef NO_STD_LIB
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <ostream>
#endif

struct Instance;
struct Block;

struct Type {
    uintptr_t tuid() const;

    virtual void renderDecl(
        std::ostream &os,
        const std::string &name
    ) const = 0;
};

template <class T>
struct TypeNative: public Type {
    virtual void renderDecl(
        std::ostream &os,
        const std::string &name
    ) const;
};

struct TypeBlock: public Type {
    Instance &parent;
    Block &block;

    inline TypeBlock(Instance &_parent, Block &_block):
        parent {_parent},
        block {_block} {}

    virtual void renderDecl(
        std::ostream &os,
        const std::string &name
    ) const;
};

struct Instance: public Type {
    Block &block;

    std::vector<std::unique_ptr<TypeBlock>> children;
    std::map<std::string, Type &> symbol_types;

    inline Instance(Block &_block):
        block {_block} {}

    Type &at(const std::string &name);
    void insert(const std::string &name, Type &type);
    Instance &lookup(const std::vector<std::string> &path);
    Type &fullLookup(
        const std::vector<std::string> &path,
        const std::string &name,
        size_t &level
    ); // also lookup parent->path->name

    void renderFuncDecl(std::ostream &os) const;
    void renderStruct(std::ostream &os) const;

    virtual void renderDecl(
        std::ostream &os,
        const std::string &name
    ) const;
};

inline bool operator==(const Type &a, const Type &b) {
    return a.tuid() == b.tuid();
}

inline bool operator!=(const Type &a, const Type &b) {
    return !(a == b);
}
