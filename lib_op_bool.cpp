#include "builtin.hpp"
#include "node.hpp"
#include "builder_lib.hpp"

namespace {

using namespace libblock;
using namespace libblock::builder;

Builtin __or__ {"core", "__or__", {
    libFuncB3R("$result = $a || $b;"),
}};

Builtin __xor__ {"core", "__xor__", {
    libFuncB3R("$result = $a ^ $b;"),
    libFuncB3A("$a = $input ^ $b;"),
    libFuncB3B("$b = $input ^ $a;"),
}};

Builtin __and__ {"core", "__and__", {
    libFuncB3R("$result = $a && $b;"),
}};

Builtin __not__ {"core", "__not__", {
    libFuncB2R("$result = !$a;"),
    libFuncB2A("$a = !$input;"),
}};

}
