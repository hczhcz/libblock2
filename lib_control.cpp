#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

class Do: public BlockBuiltin {
protected:
    virtual void inSpecialArg(
        Session &session,
        Instance &caller, Instance &,
        size_t, size_t, Node &arg
    ) {
        arg.buildProc(session, caller);
    }

    virtual void buildContent(
        Session &,
        Instance &
    ) {
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
