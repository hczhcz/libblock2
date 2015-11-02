#pragma once

#include <string>
#include <map>

struct Block;

struct Type {
    uintptr_t tuid() const;

    virtual std::string renderDecl(std::string &&name) const = 0;
};

template <class T>
struct TypeNative: public Type {
    virtual std::string renderDecl(std::string &&name) const;
};

inline bool operator==(const Type &a, const Type &b) {
    return a.tuid() == b.tuid();
}

inline bool operator!=(const Type &a, const Type &b) {
    return !(a == b);
}
