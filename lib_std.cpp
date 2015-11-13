#include "output.hpp"
#include "builder.hpp"

namespace lib {

using namespace builder;

class Then: public BlockBuiltin {
protected:
    virtual void inSpecialArg(
        Instance &caller, Instance &,
        size_t, std::unique_ptr<Node> &arg,
        Output &output,
        std::function<std::string ()> &&
    ) {
        arg->buildProc(caller, output);
    }

    virtual void buildContent(Instance &, Output &) {
        // nothing
    }

public:
    Then():
        BlockBuiltin {0, {}} {}
};
Builtin __then__ {"__then__", new Then {}};

class Set: public BlockBuiltin {
protected:
    virtual void buildContent(Instance &instance, Output &output) {
        instance.insert("dest", instance.at("src"));

        // render

        output.content(
            instance,
            [&](OutputContext &oc) {
                oc.endl();
                oc.os << instance.strCast("self") << "->dest = "
                      << instance.strCast("self") << "->src;";
            }
        );
    }

public:
    Set():
        BlockBuiltin {0, {out("dest"), in("src")}} {}
};
Builtin __set__ {"__set__", new Set {}};

class Print: public BlockBuiltin {
protected:
    virtual void buildContent(Instance &instance, Output &output) {
        // render

        output.content(
            instance,
            [&](OutputContext &oc) {
                oc.endl();
                oc.os << "printf(\"%s\", "
                      << instance.strCast("self")
                      << "->value);";
            }
        );
    }

public:
    Print():
        BlockBuiltin {0, {in("value")}} {}
};
Builtin print {"print", new Print {}};

}
