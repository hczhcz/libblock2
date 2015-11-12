#include "output.hpp"
#include "builder.hpp"

int main() {
    using namespace builder;

    BlockUser *block_p {
        block(
            {},
            call(
                $("__then__"),
                call(
                    $("__set__"), $("a"), blocks( // function a
                        block(
                            {},
                            call(
                                $("__then__"),
                                call(
                                    $("__set__"), $("c"), _("hell")
                                ),
                                call(
                                    $("__set__"), $($("parent"), "c"), _("o")
                                ),
                                call(
                                    $("__set__"), $("result"), $("self")
                                )
                            )
                        )
                    )
                ),
                call(
                    $("__set__"), $("b"), blocks( // function b
                        block(
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
                    )
                ),
                call(
                    $("__set__"), $("d"), call($("a"))
                ),
                call(
                    $("print"),
                    call($("b"), _("bye"), $($("d"), "c"))
                ),
                call(
                    call($("b"), _(1), $("print")),
                    call($("b"), _("bye"), $("c"))
                ),
                call($("print"), CallMode::static_global, _(", ")),
                call($("print"), CallMode::static_local, _("wo")),
                call($("print"), CallMode::dynamic_stack, _("rl")),
                call($("print"), CallMode::dynamic_free, _("d!"))
            )
        )
    };

    Output output;
    Instance *root_p {nullptr};

    block_p->build(
        output,
        [&](Instance &root) {
            Builtin::apply(root);
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
    fs << std::endl;
    output.getContent(fs, *root_p);

    return 0;
}
