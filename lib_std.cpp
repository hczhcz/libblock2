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
    Then():
        BlockBuiltin {{}} {}
};
// BuiltinManager::insert("__then__", new Then {});

class Set: public BlockBuiltin {
protected:
    virtual void buildContent(Instance &instance, Output &output) {
        instance.insert("dest", instance.at("src"));

        // render

        OutputContext &oc {output.content(instance)};

        oc.endl();
        oc.os << instance.strCast("self") << "->dest = "
              << instance.strCast("self") << "->src;";
    }

public:
    Set():
        BlockBuiltin {{out("dest"), in("src")}} {}
};
// BuiltinManager::insert("__set__", new Set {});

class Print: public BlockBuiltin {
protected:
    virtual void buildContent(Instance &instance, Output &output) {
        // render

        OutputContext &oc {output.content(instance)};

        oc.endl();
        oc.os << "printf(\"%s\", " << instance.strCast("self") << "->value);";
    }

public:
    Print():
        BlockBuiltin {{in("value")}} {}
};
// BuiltinManager::insert("print", new Print {});

}
