#if 0
clang++ -std=c++11 -Wall -Wextra -ferror-limit=1 self_bootstrap.cpp -o self_bootstrap.out &&
./self_bootstrap.out

exit
#endif

#include "self_syntax.hpp"

int main() {
    using namespace myparser;

    auto parsed = Parser<>::parseFile("README.md");

    PassReprFull<> repr(std::cout);
    PassHighlight<> highlight(std::cout);

    parsed->runPass(&repr);
    std::cout << std::endl;
    parsed->runPass(&highlight);

    return 0;
}
