#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

Builtin __assign__ {"core", "__assign__", {
    []() -> Block & {
        return *new (GC) BlockBuiltinFmt {
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

// __assign_or__
// __assign_xor__
// __assign_and__
// __assign_add__
// __assign_sub__
// __assign_bit_or__
// __assign_bit_xor__
// __assign_bit_and__
// __assign_mul__
// __assign_div__
// __assign_int_div__
// __assign_int_mod__
// __assign_shl__
// __assign_shr__
// __assign_ushr__
// __assign_rol__
// __assign_ror__

}
