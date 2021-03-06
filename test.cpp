#include "session.hpp"
#include "builtin.hpp"
#include "type.hpp"
#include "parser.hpp"
#include "builder_node.hpp"
#include "builder_block.hpp"

int main() {
    GC_init();

    using namespace libblock;
    using namespace libblock::builder;

    BlockUser &root_block {
        block(
            {BlockOption::allow_proc},
            {},
            parse(
                R"CODE(
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

                    b(n1) = 200 + 66 shl 4 shr 1 ushr 2 ror 5 rol 3; // n1 is 233
                    n2 + n1 = 266; // n2 is 33
                    \ var x {
                        x = x + 300;
                    } (n2); // n2 is 333

                    __do__(print(n1), print(n2));
                )CODE"
            ) // TODO: add (lookup / frame / block) modes and options in the grammar
        )
    };

    Session session;
    Instance *root_p {nullptr};

    root_block.buildEntry(
        session,
        [&](Block &, Instance &root) {
            Builtin::apply("core", root);
            Builtin::apply("io", root);
        },
        [&](Block &, Instance &root) {
            root_p = &root;
        }
    );

    std::ofstream fs {
        "./build/test.gen.c"
    };
    session.renderHeader(fs, *root_p);
    fs << std::endl;
    session.renderContent(fs, *root_p);

    return 0;
}
