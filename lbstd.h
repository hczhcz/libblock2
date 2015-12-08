#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gc/gc.h>

#define LB_BIT_WIDTH 64

#define LB_FUNC(func) \
    void (func)( \
        struct frame *self, \
        struct frame *callee, \
        struct frame *inner, \
        lb_reg_t tmp \
    )

#define LB_CALL() \
    self->func(self, callee, inner, tmp);

#define LB_ENTER(func) LB_FUNC(func) {
#define LB_EXIT() LB_CALL() }

#define LB_RETURN() LB_CALL() return;
#define LB_YIELD(func) LB_CALL() } LB_FUNC(func) {

struct frame;

typedef uint64_t    lb_reg_t;
typedef LB_FUNC(*lb_func_t);

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
