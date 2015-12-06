#include "output.hpp"
#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace libblock {

namespace {

using namespace builder;

Builtin __add__ {"__add__", {
    libFuncI3R("$result = $a + $b;"),
    libFuncI3A("$a = $input - $b;"),
    libFuncI3B("$b = $input - $a;"),
}};

Builtin __sub__ {"__sub__", {
    libFuncI3R("$result = $a - $b;"),
    libFuncI3A("$a = $input + $b;"),
    libFuncI3B("$b = $a - $input;"),
}};

Builtin __mul__ {"__mul__", {
    libFuncI3R("$result = $a * $b;"),
    // libFuncI3A("$a = $input / $b;"),
    // libFuncI3B("$b = $input / $a;"),
}};

Builtin __int_div__ {"__int_div__", {
    libFuncI3R("$result = $a / $b;"),
    // libFuncI3A("$a = $input * $b;"),
    // libFuncI3B("$b = $a / $input;"),
}};

Builtin __int_mod__ {"__int_mod__", {
    libFuncI3R("$result = $a % $b;"),
}};

Builtin __pos__ {"__pos__", {
    libFuncI2R("$result = $a;"),
    libFuncI2A("$a = $input;"),
}};

Builtin __neg__ {"__neg__", {
    libFuncI2R("$result = -$a;"),
    libFuncI2A("$a = -$input;"),
}};

Builtin __bit_or__ {"__bit_or__", {
    libFuncI3R("$result = $a | $b;"),
}};

Builtin __bit_xor__ {"__bit_xor__", {
    libFuncI3R("$result = $a ^ $b;"),
    libFuncI3A("$a = $input ^ $b;"),
    libFuncI3B("$b = $input ^ $a;"),
}};

Builtin __bit_and__ {"__bit_and__", {
    libFuncI3R("$result = $a & $b;"),
}};

Builtin __bit_not__ {"__bit_not__", {
    libFuncI2R("$result = ~$a;"),
    libFuncI2A("$a = ~$input;"),
}};

Builtin __shl__ {"__shl__", {
    libFuncI3R("$result = $a << $b;"),
    // libFuncI3A("$a = $input >> $b;"),
}};

Builtin __shr__ {"__shr__", {
    libFuncI3R("$result = $a >> $b;"),
    // libFuncI3A("$a = $input << $b;"),
}};

Builtin __ushr__ {"__ushr__", {
    libFuncI3R("$result = ((uint64_t) $a) >> $b;"),
    // libFuncI3A("$a = ((uint64_t) $input) << $b;"),
}};

Builtin __rol__ {"__rol__", {
    libFuncI3R("$result = ($a << $b) | (((uint64_t) $a) >> (63 & -$b));"),
}};

Builtin __ror__ {"__ror__", {
    libFuncI3R("$result = (((uint64_t) $a) >> $b) | ($a << (63 & -$b));"),
}};

}

}
