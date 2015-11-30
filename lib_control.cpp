#include "output.hpp"
#include "builtin.hpp"
#include "type.hpp"
#include "node.hpp"
#include "builder_block.hpp"

namespace lib {

using namespace builder;

class Then: public BlockBuiltin {
protected:
    virtual void inSpecialArg(
        Instance &caller, Instance &,
        size_t, std::unique_ptr<Node> &arg,
        Output &output,
        std::function<std::string (Type &)> &&
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

Builtin __assign__ {"__assign__", {
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
