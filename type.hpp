#pragma once

#include <string>
#include <map>

struct Block;
struct Instance;

struct Type {
    uintptr_t tuid() const;

    virtual std::string renderDecl(std::string &&name) const = 0;
};

template <class T>
struct TypeNative: public Type {
    virtual std::string renderDecl(std::string &&name) const;
};

struct TypeBlock: public Type {
    Instance &parent;
    Block &block;

    inline TypeBlock(Instance &_parent, Block &_block):
        parent {_parent},
        block {_block} {}

    virtual std::string renderDecl(std::string &&name) const;
};

inline bool operator==(const Type &a, const Type &b) {
    return a.tuid() == b.tuid();
}

inline bool operator!=(const Type &a, const Type &b) {
    return !(a == b);
}
