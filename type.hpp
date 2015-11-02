#pragma once

#include <string>
#include <map>

struct Block;

struct Type {
    uintptr_t tuid() const;

    virtual std::string typeName() = 0;
};

template <class T>
struct TypeNative: public Type {
    virtual std::string typeName();
};

inline bool operator==(const Type &a, const Type &b) {
    return a.tuid() == b.tuid();
}

inline bool operator!=(const Type &a, const Type &b) {
    return !(a == b);
}
