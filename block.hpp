#pragma once

#include "include.hpp"

namespace libblock {

class Session;
class OutputContext;
class Instance;
class Node;
class NodeBlock;

enum class FrameMode {
    static_global,
    static_local,
    dynamic_gc,
    dynamic_free
};

enum class BlockOption {
    allow_proc,
    allow_out,
    allow_in,
    manual_lock
};

enum class ParamMode {
    in, out, var, special
};

class Block {
private:
    FrameMode mode;
    std::set<BlockOption> options;
    std::gc_vector<std::pair<std::string, ParamMode>> params;

    std::gc_list<std::reference_wrapper<Instance>> instances;

    Instance &matchInstance(
        Session &session,
        Instance &instance
    );

    void renderFrame(
        Instance &caller,
        size_t position,
        OutputContext &oc
    ) const;
    void renderCall(
        Instance &caller, Instance &instance,
        size_t position,
        OutputContext &oc
    ) const;

protected:
    Block(
        // FrameMode _mode, // TODO
        std::set<BlockOption> &&_options,
        std::gc_vector<std::pair<std::string, ParamMode>> &&_params
    );

    virtual void inSpecialArg(
        Session &session,
        Instance &caller, Instance &instance,
        size_t position, size_t index, Node &arg
    );
    virtual void buildContent(
        Session &session,
        Instance &instance
    ) = 0;
    virtual void outSpecialArg(
        Session &session,
        Instance &caller, Instance &instance,
        size_t position, size_t index, Node &arg
    );

public:
    bool getOption(BlockOption option);

    void inArg(
        Session &session,
        Instance &caller, Instance &instance,
        size_t position, size_t index, Node &arg
    );
    void outArg(
        Session &session,
        Instance &caller, Instance &instance,
        size_t position, size_t index, Node &arg
    );
    void buildEntry(
        Session &session,
        std::gc_function<void (Block &, Instance &)> &&before,
        std::gc_function<void (Block &, Instance &)> &&after
    );
    void buildCall(
        Session &session,
        Instance &caller,
        size_t position,
        std::gc_function<void (Block &, Instance &)> &&before,
        std::gc_function<void (Block &, Instance &)> &&after
    );
};

class BlockBuiltin: public Block {
protected:
    static void codeFmt(
        const std::string &str,
        Instance &instance, OutputContext &oc
    );

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
    virtual void buildContent(
        Session &session,
        Instance &instance
    );

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
    virtual void buildContent(
        Session &session,
        Instance &instance
    );

public:
    BlockUser(
        std::set<BlockOption> &&_options,
        std::gc_vector<std::pair<std::string, ParamMode>> &&_params,
        Node &_ast
    );
};

}
