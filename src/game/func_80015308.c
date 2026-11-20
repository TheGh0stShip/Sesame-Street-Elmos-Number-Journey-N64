#include "common.h"

s32 func_80015308(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp0;

    sp0 = (s32)&D_80133ED0;
    (*(s32 *)sp0) = 0;
    ((struct gd_10_s8 *)sp0)->v = arg0;
    ((struct gd_11_s8 *)sp0)->v = arg1;
    ((struct gd_14_s32 *)sp0)->v = arg2;
    ((struct gd_18_s32 *)sp0)->v = arg3;
    return sp0;
}
