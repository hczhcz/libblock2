#include "output.hpp"
#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

Builtin print {"io", "print", {
    []() -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeVoid("value")},
            {},
            "printf(\"void\");"
        };
    },
    []() -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeBool("value")},
            {},
            "printf($value ? \"true\" : \"false\");"
        };
    },
    []() -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeInt("value")},
            {},
            "printf(\"%ld\", $value);"
        };
    },
    []() -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeReal("value")},
            {},
            "printf(\"%lf\", $value);"
        };
    },
    []() -> Block & {
        return *new (GC) BlockBuiltinFmt {
            {BlockOption::allow_proc},
            {in("value")},
            {typeStr("value")},
            {},
            "printf(\"%s\", $value);"
        };
    },
}};

}
