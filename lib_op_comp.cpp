#include "output.hpp"
#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace libblock {

namespace {

using namespace builder;

Builtin __eq__ {"__eq__", {
    libFuncB3R("$result = $a == $b;"),
    libFuncI3R("$result = $a == $b;"),
    libFuncR3R("$result = $a == $b;"),
}};

Builtin __ne__ {"__ne__", {
    libFuncB3R("$result = $a != $b;"),
    libFuncI3R("$result = $a != $b;"),
    libFuncR3R("$result = $a != $b;"),
}};

Builtin __lt__ {"__lt__", {
    libFuncB3R("$result = $a < $b;"),
    libFuncI3R("$result = $a < $b;"),
    libFuncR3R("$result = $a < $b;"),
}};

Builtin __lte__ {"__lte__", {
    libFuncB3R("$result = $a <= $b;"),
    libFuncI3R("$result = $a <= $b;"),
    libFuncR3R("$result = $a <= $b;"),
}};

Builtin __gt__ {"__gt__", {
    libFuncB3R("$result = $a > $b;"),
    libFuncI3R("$result = $a > $b;"),
    libFuncR3R("$result = $a > $b;"),
}};

Builtin __gte__ {"__gte__", {
    libFuncB3R("$result = $a >= $b;"),
    libFuncI3R("$result = $a >= $b;"),
    libFuncR3R("$result = $a >= $b;"),
}};

}

}
