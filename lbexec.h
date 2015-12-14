#pragma once

#include "lbstd.h"

#define LB_FUNC(func) \
    static void (func)( \
        struct frame *self, \
        struct frame *callee, \
        struct frame *inner, \
        lb_reg_t tmp \
    )

#define LB_CALL() \
    return self->func(self, callee, inner, tmp);

#define LB_ENTER(func) LB_FUNC(func) {
#define LB_EXIT() LB_CALL() }
#define LB_YIELD(func) LB_CALL() } LB_FUNC(func) {
