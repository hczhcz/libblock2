#include "output.hpp"
#include "block.hpp"
#include "builder.hpp"

#include <iostream>

int main() {
    using namespace builder;

    Block root {
        {},
        call(
            $(";"),
            call(
                $("="), $("c"), block(
                    {in("a"), in("b")},
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
