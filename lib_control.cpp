#include "output.hpp"
#include "builtin.hpp"
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
        BlockBuiltin {
            {BlockOption::allow_proc},
            {}
        } {}
};
Builtin __then__ {"__then__", {
    []() -> Block * {
        return new Then {};
    },
}};

Builtin __set__ {"__set__", {
    []() -> Block * {
        return new BlockBuiltinFmt {
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
