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
    //         $("__then__"),
    //         call(
    //             $("__set__"), $("c"), block(
    //                 {in("a"), in("b")},
    //                 call(
    //                     $("__then__"),
    //                     call(
    //                         $("__set__"), $("t"), call($("*"), _(2), $("b"))
    //                     ),
    //                     call(
    //                         $("__set__"), $("result"), call($("+"), $("a"), $("t"))
    //                     )
    //                 )
    //             )
    //         ),
    //         call($("c"), _("xx"), _("yy"))
    //     )
    // };
    BlockUser *root_p {
        block(
            {},
            call(
                $("__then__"),
                call(
                    $("__set__"), $("c"), block(
                        {in("a"), in("b")},
                        call(
                            $("__then__"),
                            call(
                                $("__set__"), $("t"), $("b")
                            ),
                            call(
                                $("__set__"), $("result"), $("t")
                            )
                        )
                    )
                ),
                call(
                    $("__set__"), $("d"), call(
                        $("c"), _("xx"), _("yy")
                    )
                )
            )
        )
    };

    Output output;

    root_p->makeBoot(
        output,
        [&](Instance &instance) {
            BlockBuiltin::applyBuiltin(instance);
        },
        [](Instance &) {
            //
        }
    );

    std::cout << "======== header ========" << std::endl;
    output.getHeader(std::cout);
    std::cout << "======== content ========" << std::endl;
    output.getContent(std::cout);

    return 0;
}
