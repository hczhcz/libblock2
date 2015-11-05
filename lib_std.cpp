#include "node.hpp"
#include "builder.hpp"

namespace {

using namespace builder;

struct Then: public BlockBuiltin {
    using BlockBuiltin::BlockBuiltin;

    virtual void inSpecialArg(
        Instance &parent, Instance &,
        size_t, std::unique_ptr<Node> &arg,
        Output &output
    ) {
        arg->buildProc(parent, output);
    }
} then {
    {},
    "__then__"
};

struct Set: public BlockBuiltin {
    using BlockBuiltin::BlockBuiltin;

    virtual void buildContent(Instance &instance, Output &) {
        instance.insert("dest", instance.at("src"));
    }
} set {
    {out("dest"), in("src")},
    "__set__"
};

}
