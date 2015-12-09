#ifndef MYPARSER_CUSTOMIZED
    #define MYPARSER_CUSTOMIZED
    #define MYPARSER_STD_REGEX
#endif

#ifndef SKIP_LIB
    #define SKIP_LIB

    #include <functional>
    #include <string>
    #include <list>
    #include <vector>
    #include <set>
    #include <map>
    #include <fstream>
    #include <iostream>

    #include <gc/gc.h>
    #include <gc/gc_cpp.h>
    #include <gc/gc_allocator.h>

    #include "./lib/fork_try.hpp"
    #include "./lib/myparser/myparser.hpp"

    extern "C" {
        #include "lbstd.h"
    }
#endif

// TODO
namespace std {
    template <class T>
    using gc_list = list<T, gc_allocator<T>>;
    template <class T>
    using gc_vector = vector<T, gc_allocator<T>>;
    template <class Key, class T>
    using gc_map = map<Key, T, gc_allocator<T>>;
}
