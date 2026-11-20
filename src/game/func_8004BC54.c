#include "common.h"


void func_8004BC54(s32 arg0, s32 arg1, s32 arg2) {
    register s32 reg_s0;
    register s32 reg_s1;

    reg_s0 = arg0;
    reg_s1 = reg_s0 + 8;
    (*(f32 *)reg_s1) = *(f32 *)arg1;
    ((struct gd_4_f32 *)reg_s1)->v = ((struct gd_4_f32 *)arg1)->v;
    ((struct gd_8_f32 *)reg_s1)->v = ((struct gd_8_f32 *)arg1)->v;
    ((struct gd_30_f32 *)reg_s0)->v = (f32)func_80069920(((f64)((struct gd_8_f32 *)arg2)->v), ((f64)(*(f32 *)arg2)));
}
