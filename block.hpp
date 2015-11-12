#pragma once

#include "include.hpp"

class Output;
class Instance;
class Node;
class NodeBlock;

enum class ParamMode {
    in, out, var, special
};

class Block {
private:
    std::vector<std::pair<std::string, ParamMode>> params;

    std::list<std::unique_ptr<Instance>> instances;

protected:
    Instance &matchInstance(
        std::unique_ptr<Instance> &&instance,
        Output &output
    );

    virtual void inSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output,
        std::function<std::string ()> &&target
    );
    virtual void buildContent(Instance &instance, Output &output) = 0;
    virtual void outSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output,
        std::function<std::string ()> &&target
    );

public:
    Block(std::vector<std::pair<std::string, ParamMode>> &&_params);

    void inArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output,
        std::function<std::string ()> &&target
    );
    void outArg(
        Instance &caller, Instance &instance,
        size_t index, std::unique_ptr<Node> &arg,
        Output &output,
        std::function<std::string ()> &&target
    );
    void build(
        Output &output,
        std::function<void (Instance &)> &&before,
        std::function<void (Instance &)> &&after
    );
};

class Builtin {
private:
    static std::map<std::string, std::shared_ptr<NodeBlock>> &builtins();

public:
    static void apply(Instance &instance);

    template <class... Blocks>
    Builtin(std::string &&name, Blocks... _blocks) {
        builtins().insert({
            std::move(name),
            std::make_shared<NodeBlock>(_blocks...)
        });
    }
};

class BlockBuiltin: public Block {
public:
    using Block::Block;
};

class BlockUser: public Block {
private:
    std::unique_ptr<Node> ast_p;

protected:
    // as block
    virtual void buildContent(Instance &instance, Output &output);

public:
    BlockUser(
        std::vector<std::pair<std::string, ParamMode>> &&_params,
        Node *_ast
    );
};
