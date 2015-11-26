#include "output.hpp"
#include "builtin.hpp"
#include "builder.hpp"

namespace lib {

using namespace builder;

Builtin __add__ {"__add__", {
    []() -> Block * {
        return new BlockBuiltinFmt {
            {BlockOption::allow_out},
            {in("a"), in("b")},
            {typeInt("a"), typeInt("b"), typeInt("result")},
            {},
            "$result = $a + $b;"
        };
    },
    []() -> Block * {
        return new BlockBuiltinFmt {
            {BlockOption::allow_in},
            {out("a"), in("b")},
            {typeInt("input"), typeInt("a"), typeInt("b")},
            {},
            "$a = $input - $b;"
        };
    },
    []() -> Block * {
        return new BlockBuiltinFmt {
            {BlockOption::allow_in},
            {in("a"), out("b")},
            {typeInt("input"), typeInt("a"), typeInt("b")},
            {},
            "$b = $input - $a;"
        };
    },
}};

}
