#include "output.hpp"
#include "builtin.hpp"
#include "builder.hpp"

namespace lib {

using namespace builder;

Builtin print {"print", {
    []() -> Block * {
        return new BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeBool("value")},
            {},
            "printf($value ? \"true\" : \"false\");"
        };
    },
    []() -> Block * {
        return new BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeInt("value")},
            {},
            "printf(\"%ld\", $value);"
        };
    },
    []() -> Block * {
        return new BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeReal("value")},
            {},
            "printf(\"%lf\", $value);"
        };
    },
    []() -> Block * {
        return new BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeStr("value")},
            {},
            "printf(\"%s\", $value);"
        };
    },
}};

}
