#include "builder.hpp"

namespace {

using namespace builder;

class Then: public BlockBuiltin {
protected:
    virtual void inSpecialArg(
        Instance &caller, Instance &,
        size_t, std::unique_ptr<Node> &arg,
        Output &output
    ) {
        arg->buildProc(caller, output);
    }

public:
    using BlockBuiltin::BlockBuiltin;
} then {
    {},
    "__then__"
};

class Set: public BlockBuiltin {
protected:
    virtual void buildContent(Instance &instance, Output &) {
        instance.insert("dest", instance.at("src"));
    }

public:
    using BlockBuiltin::BlockBuiltin;
} set {
    {out("dest"), in("src")},
    "__set__"
};

}
