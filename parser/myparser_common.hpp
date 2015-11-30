#pragma once

// option list

#if !defined(MYPARSER_CUSTOMIZED)
    #define MYPARSER_AST_CCC

    #if defined(__GNUC__) || defined(__clang__)
        #define MYPARSER_FORCE_INLINE
    #endif

    #define MYPARSER_STD_REGEX
#endif

// library

#include <limits>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#if defined(MYPARSER_BOOST_REGEX)
    #include <boost/regex.hpp>
    namespace regex_lib = boost;
#elif defined(MYPARSER_BOOST_XPRESSIVE)
    #include <boost/xpressive/xpressive_dynamic.hpp>
    namespace regex_lib = boost::xpressive;
#elif defined(MYPARSER_STD_REGEX)
    #include <regex>
    namespace regex_lib = std;
#else
    // Error!
#endif

#if defined(MYPARSER_AST_CCC)
    #include "lib/ccc.hpp"
#endif

#if defined(MYPARSER_FORCE_INLINE)
    #define MYPARSER_INLINE __attribute__((__always_inline__)) inline
#else
    #define MYPARSER_INLINE inline
#endif

namespace myparser {

#if defined(MYPARSER_DEBUG)
    template <class T>
    inline void mpDebug(T value) {
        std::cerr << value << std::endl;
    }
#else
    template <class T>
    inline void mpDebug(T) {}
#endif

#if defined(MYPARSER_AST_CCC)
    const auto style_normal = ccc::d_all;
    const auto style_underline = ccc::s_underline_single;

    const auto style_error = ccc::cf_red + ccc::s_bold;
    const auto style_space = ccc::s_faint;
    const auto style_keyword = ccc::cf_blue + ccc::s_bold;
    const auto style_data = ccc::cf_magenta + ccc::s_bold;
    const auto style_string = ccc::cf_yellow + ccc::s_bold;
#else
    const auto style_normal = "";
    const auto style_underline = ccc::s_underline_single;

    const auto style_error = "";
    const auto style_space = "";
    const auto style_keyword = "";
    const auto style_data = "";
    const auto style_string = "";
#endif

enum {
    PASS_REPR,
    PASS_HIGHLIGHT,
    PASS_MESSAGE, // TODO // input [(list, message), ...] output [lineno:pos message]
    PASS_USER,

    PASS_FIN = 64
};

// forward declaration

template <size_t>
class Pass;

}
