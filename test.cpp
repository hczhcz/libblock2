#include "output.hpp"
#include "builder.hpp"

#ifndef NO_STD_LIB
#include <iostream>
#endif

int main() {
    using namespace builder;

    // BlockUser root {
    //     {},
    //     call(
    //         $(";"),
    //         call(
    //             $("="), $("c"), block(
    //                 {in("a"), in("b")},
    //                 call(
    //                     $(";"),
    //                     call(
    //                         $("="), $("t"), call($("*"), _(2), $("b"))
    //                     ),
    //                     call(
    //                         $("="), $("result"), call($("+"), $("a"), $("t"))
    //                     )
    //                 )
    //             )
    //         ),
    //         call($("c"), _("xx"), _("yy"))
    //     )
    // };
    BlockUser root {
        {},
        call(
            $(";"),
            call(
                $("="), $("c"), block(
                    {in("a"), in("b")},
                    call(
                        $(";"),
                        call(
                            $("="), $("t"), $("b")
                        ),
                        call(
                            $("="), $("result"), $("t")
                        )
                    )
                )
            ),
            call($("c"), _("xx"), _("yy"))
        )
    };

    Output output;

    root.matchInstance(Instance {root}, output);

    return 0;
}
