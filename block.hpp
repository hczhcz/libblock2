#pragma once

#include "include.hpp"

namespace libblock {

class Output;
class Instance;
class Node;
class NodeBlock;

enum class ParamMode {
    in, out, var, special
};

enum class BlockOption {
    allow_proc,
    allow_out,
    allow_in
};

class Block {
private:
    std::set<BlockOption> options;
    std::gc_vector<std::pair<std::string, ParamMode>> params;

    std::gc_list<std::reference_wrapper<Instance>> instances;

protected:
    Instance &matchInstance(
        Instance &instance,
        Output &output
    );

    virtual void inSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, Node &arg,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
    virtual void buildContent(Instance &instance, Output &output) = 0;
    virtual void outSpecialArg(
        Instance &caller, Instance &instance,
        size_t index, Node &arg,
        Output &output,
        std::function<std::string (Type &)> &&target
    );

public:
    Block(
        std::set<BlockOption> &&_options,
        std::gc_vector<std::pair<std::string, ParamMode>> &&_params
    );

    bool getOption(BlockOption option);

    void inArg(
        Instance &caller, Instance &instance,
        size_t index, Node &arg,
        Output &output,
        std::function<std::string (Type &)> &&target
    );
    void outArg(
        Instance &caller, Instance &instance,
        size_t index, Node &arg,
        Output &output,
        std::function<std::string (Type &)> &&target
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
    std::gc_map<
        std::string,
        std::reference_wrapper<Type>
    > static_types;
    std::gc_vector<std::pair<std::string, std::string>> dynamic_types;
    std::string str;

protected:
    // as block
    virtual void buildContent(Instance &instance, Output &output);

public:
    BlockBuiltinFmt(
        std::set<BlockOption> &&_options,
        std::gc_vector<std::pair<std::string, ParamMode>> &&_params,
        std::gc_map<
            std::string,
            std::reference_wrapper<Type>
        > &&_static_types,
        std::gc_vector<std::pair<std::string, std::string>> &&_dynamic_types,
        std::string &&_str
    );
};

class BlockUser: public Block {
private:
    Node &ast;

protected:
    // as block
    virtual void buildContent(Instance &instance, Output &output);

public:
    BlockUser(
        std::set<BlockOption> &&_options,
        std::gc_vector<std::pair<std::string, ParamMode>> &&_params,
        Node &_ast
    );
};

}
