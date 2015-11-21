#include "output.hpp"
#include "type.hpp"
#include "builder.hpp"

namespace lib {

using namespace builder;

Builtin print {"print",
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {
            {"value", TypeNativeBool::get()}
        },
        {},
        "printf($value ? \"true\" : \"false\");"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {
            {"value", TypeNativeInt::get()}
        },
        {},
        "printf(\"%ld\", $value);"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {
            {"value", TypeNativeReal::get()}
        },
        {},
        "printf(\"%lf\", $value);"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {
            {"value", TypeNativeStr::get()}
        },
        {},
        "printf(\"%s\", $value);"
    }
};

}
