#include "output.hpp"
#include "builder.hpp"

#ifndef NO_STD_LIB
#include <fstream>
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
    BlockUser *block_p {
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
    Instance *root_p {nullptr};

    block_p->build(
        output,
        [&](Instance &root) {
            BlockBuiltin::applyBuiltin(root);
        },
        [&](Instance &root) {
            root_p = &root;
        }
    );

    std::cout << "======== header ========" << std::endl;
    output.getHeader(std::cout, *root_p);
    std::cout << "======== content ========" << std::endl;
    output.getContent(std::cout, *root_p);

    std::fstream fs {
        "./build/test.gen.c", std::fstream::out
    };
    output.getHeader(fs, *root_p);
    output.getContent(fs, *root_p);

    return 0;
}
