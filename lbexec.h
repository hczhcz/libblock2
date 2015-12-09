#pragma once

#include "lbstd.h"

#define LB_CALL() \
    self->func(self, callee, inner, tmp);

#define LB_ENTER(func) LB_FUNC(func) {
#define LB_EXIT() LB_CALL() }

#define LB_RETURN() LB_CALL() return;
#define LB_YIELD(func) LB_CALL() } LB_FUNC(func) {
