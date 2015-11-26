#pragma once

#include "include.hpp"

class Output;
class Instance;
class Node;
class NodeBlock;

enum class ParamMode {
    in, out, var, special
};

enum class BlockOption {
    parent,
    allow_proc,
    allow_out,
    allow_in
};

class Block {
private:
    std::set<BlockOption> options;
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
    Block(
        std::set<BlockOption> &&_options,
        std::vector<std::pair<std::string, ParamMode>> &&_params
    );

    bool getOption(BlockOption option);

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

class BlockBuiltin: public Block {
public:
    using Block::Block;
};

class BlockBuiltinFmt: public BlockBuiltin {
private:
    std::map<std::string, Type &> static_types;
    std::vector<std::pair<std::string, std::string>> dynamic_types;
    std::string str;

protected:
    // as block
    virtual void buildContent(Instance &instance, Output &output);

public:
    BlockBuiltinFmt(
        std::set<BlockOption> &&_options,
        std::vector<std::pair<std::string, ParamMode>> &&_params,
        std::map<std::string, Type &> &&_static_types,
        std::vector<std::pair<std::string, std::string>> &&_dynamic_types,
        std::string &&_str
    );
};

class BlockUser: public Block {
private:
    std::unique_ptr<Node> ast_p;

protected:
    // as block
    virtual void buildContent(Instance &instance, Output &output);

public:
    BlockUser(
        std::set<BlockOption> &&_options,
        std::vector<std::pair<std::string, ParamMode>> &&_params,
        Node *_ast
    );
};
