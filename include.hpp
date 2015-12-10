#ifndef MYPARSER_CUSTOMIZED
    #define MYPARSER_CUSTOMIZED
    #define MYPARSER_STD_REGEX
#endif

#ifndef SKIP_LIB
    #define SKIP_LIB

    #include <functional>
    #include <string>
    #include <set>
    #include <fstream>
    #include <iostream>

    #include "./lib/gc_util.hpp"
    #include "./lib/fork_try.hpp"
    #include "./lib/myparser/myparser.hpp"

    extern "C" {
        #include "lbstd.h"
    }
#endif
