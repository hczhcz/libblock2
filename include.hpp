#ifndef SKIP_LIB
    #define SKIP_LIB

    #include <memory>
    #include <functional>
    #include <string>
    #include <list>
    #include <vector>
    #include <set>
    #include <map>
    #include <fstream>
    #include <iostream>

    #include "./lib/fork_try.hpp"
    #include "./lib/delayed_call.hpp"

    #define MYPARSER_CUSTOMIZED
    #define MYPARSER_STD_REGEX
    #include "./lib/myparser/myparser.hpp"
#endif
