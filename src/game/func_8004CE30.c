#include "common.h"

void func_8004CE30(s32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    register s32 reg_v0;

    if (arg0 == 0) {
        return;
    }
    reg_v0 = arg0;
    ((struct gd_18_s32 *)reg_v0)->v = 0;
    ((struct gd_10_s32 *)reg_v0)->v = 0;
    ((struct gd_8_s32 *)reg_v0)->v = 0;
    if ((arg3 & 2) != 0) {
        ((struct gd_14_s32 *)reg_v0)->v = 0;
        ((struct gd_C_s32 *)reg_v0)->v = 0;
        ((struct gd_4_s32 *)reg_v0)->v = 0;
        return;
    }
    if ((arg3 & 1) == 0) {
        return;
    }
    if ((arg3 & 4) != 0) {
        ((struct gd_8_f32 *)reg_v0)->v = arg2;
    } else {
        ((struct gd_10_f32 *)reg_v0)->v = arg2;
    }
    ((struct gd_18_f32 *)reg_v0)->v = arg1;
}
