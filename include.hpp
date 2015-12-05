#define MYPARSER_CUSTOMIZED
#define MYPARSER_STD_REGEX

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

    // TODO: use GC
    #include <gc/gc.h>
    #include <gc/gc_cpp.h>
    #include <gc/gc_allocator.h>

    #include "./lib/fork_try.hpp"
    #include "./lib/myparser/myparser.hpp"
#endif
