#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gc/gc.h>

typedef uint64_t    lb_reg_t;
typedef void *      lb_func_t;

#ifdef __cplusplus
    typedef std::nullptr_t lb_void_t;
#else
    typedef struct {} lb_void_t;
#endif
typedef bool        lb_bool_t;
typedef int64_t     lb_int_t;
typedef uint64_t    lb_uint_t; // notice: for bit ops only
typedef double      lb_real_t;

struct frame {
    lb_func_t func;
    struct frame *caller;
    struct frame *outer;
};
