#include "builtin.hpp"
#include "output.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

Builtin __add__ {"core", "__add__", {
    libFuncI3R("$result = $a + $b;"),
    libFuncI3A("$a = $input - $b;"),
    libFuncI3B("$b = $input - $a;"),
}};

Builtin __sub__ {"core", "__sub__", {
    libFuncI3R("$result = $a - $b;"),
    libFuncI3A("$a = $input + $b;"),
    libFuncI3B("$b = $a - $input;"),
}};

Builtin __mul__ {"core", "__mul__", {
    libFuncI3R("$result = $a * $b;"),
    // libFuncI3A("$a = $input / $b;"),
    // libFuncI3B("$b = $input / $a;"),
}};

Builtin __int_div__ {"core", "__int_div__", {
    libFuncI3R("$result = $a / $b;"),
    // libFuncI3A("$a = $input * $b;"),
    // libFuncI3B("$b = $a / $input;"),
}};

Builtin __int_mod__ {"core", "__int_mod__", {
    libFuncI3R("$result = $a % $b;"),
}};

Builtin __pos__ {"core", "__pos__", {
    libFuncI2R("$result = $a;"),
    libFuncI2A("$a = $input;"),
}};

Builtin __neg__ {"core", "__neg__", {
    libFuncI2R("$result = -$a;"),
    libFuncI2A("$a = -$input;"),
}};

Builtin __bit_or__ {"core", "__bit_or__", {
    libFuncI3R("$result = $a | $b;"),
}};

Builtin __bit_xor__ {"core", "__bit_xor__", {
    libFuncI3R("$result = $a ^ $b;"),
    libFuncI3A("$a = $input ^ $b;"),
    libFuncI3B("$b = $input ^ $a;"),
}};

Builtin __bit_and__ {"core", "__bit_and__", {
    libFuncI3R("$result = $a & $b;"),
}};

Builtin __bit_not__ {"core", "__bit_not__", {
    libFuncI2R("$result = ~$a;"),
    libFuncI2A("$a = ~$input;"),
}};

Builtin __shl__ {"core", "__shl__", {
    libFuncI3R("$result = $a << $b;"),
    // libFuncI3A("$a = $input >> $b;"),
}};

Builtin __shr__ {"core", "__shr__", {
    libFuncI3R("$result = $a >> $b;"),
    libFuncI3A("$a = $input << $b;"),
}};

Builtin __ushr__ {"core", "__ushr__", {
    libFuncI3R("$result = ((lb_uint_t) $a) >> $b;"),
    libFuncI3A("$a = $input << $b;"),
}};

Builtin __rol__ {"core", "__rol__", {
    libFuncI3R(
        "$result =\n"
            "\t($a << $b) |\n"
            "\t(((lb_uint_t) $a) >> ((LB_BIT_WIDTH - 1) & -$b));"
    ),
    libFuncI3A(
        "$a =\n"
            "\t(((lb_uint_t) $input) >> $b) |\n"
            "\t($input << ((LB_BIT_WIDTH - 1) & -$b));"
    ),
}};

Builtin __ror__ {"core", "__ror__", {
    libFuncI3R(
        "$result =\n"
            "\t(((lb_uint_t) $a) >> $b) |\n"
            "\t($a << ((LB_BIT_WIDTH - 1) & -$b));"
    ),
    libFuncI3A(
        "$a =\n"
            "\t($input << $b) |\n"
            "\t(((lb_uint_t) $input) >> ((LB_BIT_WIDTH - 1) & -$b));"
    ),
}};

}
