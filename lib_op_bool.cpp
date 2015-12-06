#include "output.hpp"
#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace libblock {

namespace {

using namespace builder;

Builtin __or__ {"__or__", {
    libFuncB3R("$result = $a || $b;"),
}};

Builtin __xor__ {"__xor__", {
    libFuncB3R("$result = $a ^ $b;"),
    libFuncB3A("$a = $input ^ $b;"),
    libFuncB3B("$b = $input ^ $a;"),
}};

Builtin __and__ {"__and__", {
    libFuncB3R("$result = $a && $b;"),
}};

Builtin __not__ {"__not__", {
    libFuncB2R("$result = !$a;"),
    libFuncB2A("$a = !$input;"),
}};

}

}
