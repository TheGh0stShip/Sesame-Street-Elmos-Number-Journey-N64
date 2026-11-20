#include "common.h"

void func_800441EC(s32 arg0, s32 arg1, s32 arg2) {
    f32 sp0;
    f32 sp4;
    f32 sp8;

    sp0 = *(f32 *)arg1;
    sp4 = ((struct gd_4_f32 *)arg1)->v;
    sp8 = ((struct gd_8_f32 *)arg1)->v;
    (*(f32 *)arg0) = ((((*(f32 *)arg2) * sp0) + ((*(f32 *)(arg2 + 16)) * sp4)) + ((*(f32 *)(arg2 + 32)) * sp8)) + (*(f32 *)(arg2 + 48));
    ((struct gd_4_f32 *)arg0)->v = ((((*(f32 *)(arg2 + 4)) * sp0) + ((*(f32 *)(arg2 + 20)) * sp4)) + ((*(f32 *)(arg2 + 36)) * sp8)) + (*(f32 *)(arg2 + 52));
    ((struct gd_8_f32 *)arg0)->v = ((((*(f32 *)(arg2 + 8)) * sp0) + ((*(f32 *)(arg2 + 24)) * sp4)) + ((*(f32 *)(arg2 + 40)) * sp8)) + (*(f32 *)(arg2 + 56));
}
