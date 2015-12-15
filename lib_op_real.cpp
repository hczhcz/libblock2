#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

Builtin __add__ {"core", "__add__", {
    libFuncR3R("$result = $a + $b;"),
    libFuncR3A("$a = $input - $b;"),
    libFuncR3B("$b = $input - $a;"),
}};

Builtin __sub__ {"core", "__sub__", {
    libFuncR3R("$result = $a - $b;"),
    libFuncR3A("$a = $input + $b;"),
    libFuncR3B("$b = $a - $input;"),
}};

Builtin __mul__ {"core", "__mul__", {
    libFuncR3R("$result = $a * $b;"),
    libFuncR3A("$a = $input / $b;"),
    libFuncR3B("$b = $input / $a;"),
}};

Builtin __div__ {"core", "__div__", {
    libFuncR3R("$result = $a / $b;"),
    libFuncR3A("$a = $input * $b;"),
    libFuncR3B("$b = $a / $input;"),
}};

Builtin __pos__ {"core", "__pos__", {
    libFuncR3R("$result = $a;"),
    libFuncR3A("$a = $input;"),
}};

Builtin __neg__ {"core", "__neg__", {
    libFuncR3R("$result = -$a;"),
    libFuncR3A("$a = -$input;"),
}};

// TODO: div / mod for float?

}
