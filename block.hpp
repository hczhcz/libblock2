#pragma once

#include "include.hpp"

class Output;
class Instance;
class Node;
class NodeBlock;

enum class SymbolMode {
    in, out, var, special
};

class Block {
private:
    std::vector<std::pair<std::string, SymbolMode>> params;

    std::vector<std::unique_ptr<Instance>> instances;

protected:
    Instance &matchInstance(
        std::unique_ptr<Instance> &&instance,
        Output &output
    );

    virtual void inSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );
    virtual void buildContent(Instance &instance, Output &output) = 0;
    virtual void outSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );

public:
    Block(std::vector<std::pair<std::string, SymbolMode>> &&_params);

    void inArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );
    void outArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output, const std::string &target
    );
    void build(
        Output &output,
        std::function<void (Instance &)> &&before,
        std::function<void (Instance &)> &&after
    );
};

class BuiltinManager {
private:
    static std::map<std::string, std::shared_ptr<NodeBlock>> builtins();

public:
    template <class... Blocks>
    static void insert(std::string &&name, Blocks... _blocks) {
        builtins().insert({
            std::move(name),
            std::make_shared<NodeBlock>(_blocks...)
        });
    }
    static void apply(Instance &instance);
};

class BlockBuiltin: public Block {
public:
    using Block::Block;
};

class BlockUser: public Block {
private:
    std::unique_ptr<Node> ast;

protected:
    // as block
    virtual void buildContent(Instance &instance, Output &output);

public:
    BlockUser(
        std::vector<std::pair<std::string, SymbolMode>> &&_params,
        Node *_ast
    );
};
