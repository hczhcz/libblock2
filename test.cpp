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
                    $("print"),
                    call(
                        $("c"), _("bye"), _("hello")
                    )
                ),
                call(
                    $("print"),
                    _(", world")
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
