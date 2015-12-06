#include "output.hpp"
#include "builtin.hpp"
#include "node.hpp"
#include "builder_type.hpp"
#include "builder_block.hpp"
#include "builder_lib.hpp"

namespace libblock {

namespace lib {

using namespace builder;

Builtin __add__ {"__add__", {
    libFunc3R("$result = $a + $b;"),
    libFunc3A("$a = $input - $b;"),
    libFunc3B("$b = $input - $a;"),
}};

}

}
