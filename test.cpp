#include "util.hpp"
#include "output.hpp"
#include "block.hpp"
#include "builder.hpp"

#include <iostream>

int main() {
    using namespace builder;

    Block root {
        {},
        {tmp("c")},
        call(
            $(";"),
            call(
                $("="), $("c"), block(
                    {"a", "b"},
                    {in("a"), in("b"), tmp("t")},
                    call(
                        $(";"),
                        call(
                            $("="), $("t"), call($("*"), _(2), $("b"))
                        ),
                        call(
                            $("="), $("result"), call($("+"), $("a"), $("t"))
                        )
                    )
                )
            ),
            call($("c"), _("xx"), _("yy"))
        )
    };

    Output output;

    root.getInstance(Instance {root}, output);

    return 0;
}
