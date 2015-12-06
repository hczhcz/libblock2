#include "output.hpp"
#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace libblock {

namespace {

using namespace builder;

Builtin __add__ {"__add__", {
    libFuncR3R("$result = $a + $b;"),
    libFuncR3A("$a = $input - $b;"),
    libFuncR3B("$b = $input - $a;"),
}};

Builtin __sub__ {"__sub__", {
    libFuncR3R("$result = $a - $b;"),
    libFuncR3A("$a = $input + $b;"),
    libFuncR3B("$b = $a - $input;"),
}};

Builtin __mul__ {"__mul__", {
    libFuncR3R("$result = $a * $b;"),
    libFuncR3A("$a = $input / $b;"),
    libFuncR3B("$b = $input / $a;"),
}};

Builtin __div__ {"__div__", {
    libFuncR3R("$result = $a / $b;"),
    libFuncR3A("$a = $input * $b;"),
    libFuncR3B("$b = $a / $input;"),
}};

Builtin __pos__ {"__pos__", {
    libFuncR3R("$result = $a;"),
    libFuncR3A("$a = $input;"),
}};

Builtin __neg__ {"__neg__", {
    libFuncR3R("$result = -$a;"),
    libFuncR3A("$a = -$input;"),
}};

// TODO: div / mod for float?

}

}
