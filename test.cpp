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
    BlockUser *root {
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
                call($("c"), _("xx"), _("yy"))
            )
        )
    };

    Output output;

    std::unique_ptr<Instance> instance {
        new Instance{*root}
    };
    BlockBuiltin::applyBuiltin(*instance);

    root->matchInstance(std::move(instance), output);

    std::cout << "======== header ========" << std::endl;
    output.getHeader(std::cout);
    std::cout << "======== content ========" << std::endl;
    output.getContent(std::cout);

    return 0;
}
