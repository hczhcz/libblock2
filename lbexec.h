#pragma once

#include "lbstd.h"

#define LB_FUNC(func) \
    void (func)( \
        struct frame *self, \
        struct frame *callee, \
        struct frame *inner \
    )

#define LB_CALL() \
    return self->func(self, callee, inner);

#define LB_ENTER(func) \
    LB_FUNC(func) { \
        lb_reg_t tmp; \
        (void) tmp;

#define LB_EXIT() \
        LB_CALL() \
    }

#define LB_YIELD(func) LB_EXIT() LB_ENTER(func)
