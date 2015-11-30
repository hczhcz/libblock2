#pragma once

#include <iostream>

#if defined(__unix) || defined(__unix__) || defined(unix) ||\
    defined(__linux) || defined(__linux__) || defined(linux) ||\
    defined(__MACH__) || defined(BSD) || defined(__GNU__)
        #define CCC_UNIX
#endif

#if defined(_WIN64) || defined(_WIN32) || defined(_WIN16) ||\
    defined(__WIN64__) || defined(__WIN32__) || defined(__WIN16__) ||\
    defined(WINDOWS) || defined(__WINDOWS__)
        #define CCC_WINDOWS
        #include <windows.h>
#endif

namespace ccc {

enum Style {
    // font
    f_0 = 10,
    f_1,
    f_2,
    f_3,
    f_4,
    f_5,
    f_6,
    f_7,
    f_8,
    f_9,

    // foreground color
    cf_black = 30,
    cf_red,
    cf_green,
    cf_yellow,
    cf_blue,
    cf_magenta,
    cf_cyan,
    cf_white,

    // background color
    cb_black = 40,
    cb_red,
    cb_green,
    cb_yellow,
    cb_blue,
    cb_magenta,
    cb_cyan,
    cb_white,

    // enable style
    s_bold = 1,
    s_faint = 2,

    s_italic = 3,
    s_fraktur = 20,

    s_underline_single = 4,
    s_underline_double = 21,

    s_blink_slow = 5,
    s_blink_rapid = 6,

    s_framed = 51,
    s_encircled = 52,

    s_negative = 7,
    s_conceal = 8,
    s_delete = 9,
    s_overlined = 53,

    // disable style
    d_all = 0,
    d_font = 10,
    d_color_f = 39,
    d_color_b = 49,

    d_intensity = 22,
    d_fontstyle = 23,
    d_underline = 24,
    d_blink = 25,
    d_border = 54,

    d_negative = 27,
    d_conceal = 28,
    d_delete = 29,
    d_overlined = 55
};

static std::ostream &operator<<(std::ostream &s, const Style &style) {
    #if defined(CCC_UNIX)
        return s << '\x1b' << '[' << (int) style << 'm';
    #elif defined(CCC_WINDOWS)
        // not implemented

        return s;
    #endif
}

template <class T>
class alsoStyle {
public:
    const T _object;
    const Style _style;

    inline alsoStyle(const T &object, const Style &style):
        _object(object), _style(style) {}
};

template <class T>
static const alsoStyle<T> operator+(const T &object, const Style &style) {
    return alsoStyle<T>(object, style);
}

template <class T>
inline std::ostream &operator<<(std::ostream &s, const alsoStyle<T> &target) {
    return s << target._object << target._style;
}

}
