#include "output.hpp"
#include "builtin.hpp"
#include "type.hpp"
#include "builder_node.hpp"
#include "builder_block.hpp"

int main() {
    using namespace builder;

    BlockUser *block_p {
        block(
            {BlockOption::allow_proc},
            {},
            call(
                $("__then__"),
                call($("__assign__"), $("c"), "what"_lit),
                call(
                    $("__assign__"), $("a"), blocks( // function a
                        block(
                            {},
                            call(
                                $("__then__"),
                                call($("__assign__"), $("c"), "hell"_lit),
                                call($("__assign__"), $($("parent"), "c"), "o"_lit),
                                call($("__assign__"), $("result"), $("self"))
                            )
                        )
                    )
                ),
                call(
                    $("__assign__"), $("b"), blocks( // function b
                        block(
                            {in("a"), in("b")},
                            call(
                                $("__then__"),
                                call($("__assign__"), $("t"), $("b")),
                                call($("__assign__"), $("result"), $("t"))
                            )
                        )
                    )
                ),

                call(
                    $("__assign__"), $("d"), call($("a"))
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
                call($("print"), FrameMode::dynamic_free, "d!"_lit),

                call(
                    $("__assign__"),
                    $("n1"), call($("__add__"), 200_lit, 33_lit)
                ),
                call(
                    $("__assign__"),
                    call($("__add__"), $("n2"), $("n1")), 566_lit
                ),
                call($("print"), $("n1")),
                call($("print"), $("n2"))
            )
        )
    };

    Output output;
    Instance *root_p {nullptr};
    BuiltinContainer bc;

    block_p->build(
        output,
        [&](Instance &root) {
            bc.apply(root);
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
