#include "common.h"

void func_80044018(s32 arg0, s32 arg1, s32 arg2) {
    f32 sp0;
    f32 sp4;
    f32 sp8;

    sp0 = (((struct gd_4_f32 *)arg1)->v * ((struct gd_8_f32 *)arg2)->v) - (((struct gd_8_f32 *)arg1)->v * ((struct gd_4_f32 *)arg2)->v);
    sp4 = (((struct gd_8_f32 *)arg1)->v * (*(f32 *)arg2)) - ((*(f32 *)arg1) * ((struct gd_8_f32 *)arg2)->v);
    sp8 = ((*(f32 *)arg1) * ((struct gd_4_f32 *)arg2)->v) - (((struct gd_4_f32 *)arg1)->v * (*(f32 *)arg2));
    (*(f32 *)arg0) = -sp0;
    (*(f32 *)(arg0 + 4)) = -sp4;
    (*(f32 *)(arg0 + 8)) = -sp8;
    (*(s32 *)(arg0 + 12)) = 0;
    (*(f32 *)(arg0 + 16)) = -(*(f32 *)arg1);
    (*(f32 *)(arg0 + 20)) = -((struct gd_4_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 24)) = -((struct gd_8_f32 *)arg1)->v;
    (*(s32 *)(arg0 + 28)) = 0;
    (*(f32 *)(arg0 + 32)) = *(f32 *)arg2;
    (*(f32 *)(arg0 + 36)) = ((struct gd_4_f32 *)arg2)->v;
    (*(f32 *)(arg0 + 40)) = ((struct gd_8_f32 *)arg2)->v;
    (*(s32 *)(arg0 + 44)) = 0;
    (*(s32 *)(arg0 + 48)) = 0;
    (*(s32 *)(arg0 + 52)) = 0;
    (*(s32 *)(arg0 + 56)) = 0;
    (*(f32 *)(arg0 + 60)) = 1.0f;
}
