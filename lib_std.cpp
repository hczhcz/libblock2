#include "node.hpp"
#include "builder.hpp"

namespace {

using namespace builder;

struct Semicolon: public BlockBuiltin {
    using BlockBuiltin::BlockBuiltin;

    virtual void inSpecialArg(
        Instance &parent, Instance &,
        size_t, std::unique_ptr<Node> &arg,
        Output &output
    ) {
        arg->buildProc(parent, output);
    }
} semicolon {
    {},
    ";"
};

struct Assign: public BlockBuiltin {
    using BlockBuiltin::BlockBuiltin;

    virtual void buildContent(Instance &instance, Output &) {
        instance.insert("dest", instance.at("src"));
    }
} assign {
    {out("dest"), in("src")},
    "="
};

}
