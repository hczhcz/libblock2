#include "output.hpp"
#include "builder.hpp"

namespace lib {

using namespace builder;

Builtin print {
    "print",
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {typeBool("value")},
        {},
        "printf($value ? \"true\" : \"false\");"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {typeInt("value")},
        {},
        "printf(\"%ld\", $value);"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {typeReal("value")},
        {},
        "printf(\"%lf\", $value);"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_proc},
        {in("value")},
        {typeStr("value")},
        {},
        "printf(\"%s\", $value);"
    },
};

}
