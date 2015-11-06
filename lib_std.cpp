#include "output.hpp"
#include "builder.hpp"

namespace {

using namespace builder;

class Then: public BlockBuiltin {
protected:
    virtual void inSpecialArg(
        Instance &caller, Instance &,
        size_t, std::unique_ptr<Node> &arg,
        Output &output, const std::string &
    ) {
        arg->buildProc(caller, output);
    }

    virtual void buildContent(Instance &, Output &) {
        // nothing
    }

public:
    using BlockBuiltin::BlockBuiltin;
} then {
    {},
    "__then__"
};

class Set: public BlockBuiltin {
protected:
    virtual void buildContent(Instance &instance, Output &output) {
        instance.insert("dest", instance.at("src"));

        // render

        std::ostream &os {output.osContent(instance)};

        os << "    self->dest = self->src;\n";
    }

public:
    using BlockBuiltin::BlockBuiltin;
} set {
    {out("dest"), in("src")},
    "__set__"
};

}
