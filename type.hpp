#pragma once

#ifndef NO_STD_LIB
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <ostream>
#endif

class Output;
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
    virtual std::string decl(const std::string &name) const = 0;
};

template <class T>
class TypeNative: public Type {
public:
    virtual std::string decl(const std::string &name) const;
};

class TypeBlock: public Type {
private:
    Instance &parent;
    Block &block;

public:
    inline TypeBlock(Instance &_parent, Block &_block):
        parent {_parent},
        block {_block} {}

    void call(
        Output &output,
        std::function<void (Instance &, Block &)> &&before,
        std::function<void (Instance &, Block &)> &&after
    );

    virtual std::string decl(const std::string &name) const;
};

class Instance: public Type {
private:
    std::vector<std::unique_ptr<TypeBlock>> children;
    std::map<std::string, Type &> symbol_types;

    std::string tuidFunc() const;
    std::string tuidObj() const;

    friend class Block;
    friend class BlockBuiltin; // TODO: ?
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

    void renderStruct(std::ostream &os) const;
    void renderFuncDecl(std::ostream &os) const;
    void renderFuncCall(
        std::ostream &os,
        const std::string &self
    ) const;

    virtual std::string decl(const std::string &name) const;
};
