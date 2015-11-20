#include "output.hpp"
#include "builder.hpp"

int main() {
    using namespace builder;

    BlockUser *block_p {
        block(
            {BlockOption::allow_proc},
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
                                    $("__set__"), $("c"), "hell"_lit
                                ),
                                call(
                                    $("__set__"), $($("parent"), "c"), "o"_lit
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
                    call($("b"), "bye"_lit, $($("d"), "c"))
                ),
                call(
                    call($("b"), 1_lit, $("print")),
                    call($("b"), "bye"_lit, $("c"))
                ),
                call($("print"), FrameMode::static_global, ", "_lit),
                call($("print"), FrameMode::static_local, "wo"_lit),
                call($("print"), FrameMode::dynamic_stack, "rl"_lit),
                call($("print"), FrameMode::dynamic_free, "d!"_lit)
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

    std::fstream fs {
        "./build/test.gen.c", std::fstream::out
    };
    output.getHeader(fs, *root_p);
    fs << std::endl;
    output.getContent(fs, *root_p);

    return 0;
}
