#include "output.hpp"
#include "type.hpp"
#include "builder.hpp"

namespace lib {

using namespace builder;

Builtin __add__ {"__add__",
    new BlockBuiltinFmt {
        {BlockOption::allow_out},
        {in("a"), in("b")},
        {
            {"a", TypeNativeInt::get()},
            {"b", TypeNativeInt::get()},
            {"result", TypeNativeInt::get()},
        },
        {},
        "$result = $a + $b;"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_in},
        {out("a"), in("b")},
        {
            {"input", TypeNativeInt::get()},
            {"a", TypeNativeInt::get()},
            {"b", TypeNativeInt::get()},
        },
        {},
        "$a = $input - $b;"
    },
    new BlockBuiltinFmt {
        {BlockOption::allow_in},
        {in("a"), out("b")},
        {
            {"input", TypeNativeInt::get()},
            {"a", TypeNativeInt::get()},
            {"b", TypeNativeInt::get()},
        },
        {},
        "$b = $input - $a;"
    },
};

}
