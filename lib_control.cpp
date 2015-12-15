#include "builtin.hpp"
#include "output.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

class Do: public BlockBuiltin {
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
    Do():
        BlockBuiltin {
            {BlockOption::allow_proc},
            {}
        } {}
};
Builtin __do__ {"core", "__do__", {
    libFuncT<Do>()
}};

}
