#include "output.hpp"
#include "builtin.hpp"
#include "type.hpp"
#include "parser.hpp"
#include "builder_node.hpp"
#include "builder_block.hpp"

int main() {
    using namespace libblock;
    using namespace libblock::builder;

    BlockUser *block_p {
        block(
            {BlockOption::allow_proc},
            {},
            parse(
                R"CODE(
                    c = "what"; // type hint

                    a = \ {
                        c = "hell";
                        parent.c = "o";
                        result = self;
                    };
                    b = \ a, b {
                        t = b;
                        result = t;
                    } \ out a {
                        a = input;
                    }; // overloading

                    print(b("bye", a().c)); // print "hell" here
                    b(1, print)(b("wtf", c)); // print "o" here

                    b(n1) = 200 + 33; // n1 is 233
                    n2 + n1 = 266; // n2 is 33
                    \ var x {
                        x = x + 300;
                    } (n2); // n2 is 333

                    __then__(print(n1), print(n2));
                )CODE"
            ) // TODO: add (lookup / frame / block) modes and options in the grammar
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

    std::ofstream fs {
        "./build/test.gen.c"
    };
    output.getHeader(fs, *root_p);
    fs << std::endl;
    output.getContent(fs, *root_p);

    return 0;
}
