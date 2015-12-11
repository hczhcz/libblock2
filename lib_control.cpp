#include "output.hpp"
#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

class Then: public BlockBuiltin {
protected:
    virtual void inSpecialArg(
        Instance &caller, Instance &,
        size_t, Node &arg,
        Output &output,
        std::gc_function<std::string (Type &)> &&
    ) {
        arg.buildProc(caller, output);
    }

    virtual void buildContent(Instance &, Output &) {
        // nothing
    }

public:
    Then():
        BlockBuiltin {
            {BlockOption::allow_proc},
            {}
        } {}
};
Builtin __then__ {"core", "__then__", {
    libFuncT<Then>()
}};

Builtin __assign__ {"core", "__assign__", {
    []() -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {out("dest"), in("src")},
            {},
            {
                {"dest", "src"},
            },
            "$dest = $src;"
        };
    },
}};

}
