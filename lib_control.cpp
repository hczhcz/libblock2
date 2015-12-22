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

class Skip: public BlockBuiltin {
protected:
    virtual void inSpecialArg(
        Session &session,
        Instance &caller, Instance &,
        size_t, size_t, Node &arg
    ) {
        caller.content.insert(
            [&](OutputContext &oc) {
                oc.endl();
                oc.os << "/*";
                oc.enter();
            }
        );

        arg.buildProc(session, caller);

        caller.content.insert(
            [&](OutputContext &oc) {
                oc.leave();
                oc.endl();
                oc.os << "*/";
            }
        );
    }

    virtual void buildContent(
        Session &,
        Instance &
    ) {
        // nothing
    }

public:
    Skip():
        BlockBuiltin {
            {BlockOption::allow_proc},
            {}
        } {}
};
Builtin __skip__ {"core", "__skip__", {
    libFuncT<Skip>()
}};

}
