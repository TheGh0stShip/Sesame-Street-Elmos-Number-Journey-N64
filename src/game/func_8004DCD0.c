#include "common.h"

void func_8004DCD0(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp0;
    s32 sp4;

    if (arg0 == 0) {
        return;
    }
    sp0 = arg0;
    sp4 = *(s32 *)sp0;
    ((struct gd_8_s32 *)sp0)->v = arg1;
    ((struct gd_8_s32 *)sp0)->v = arg2;
    if ((arg2 >= 0) || (((arg1 < arg2) && (arg2 < 0)) || (((-arg2) < arg1) && (arg2 < 0)))) {
        ((struct gd_C_s32 *)sp0)->v = 0;
        ((struct gd_FC_f32 *)sp4)->v = ((struct gd_1C_f32 *)sp4)->v;
    }
}
