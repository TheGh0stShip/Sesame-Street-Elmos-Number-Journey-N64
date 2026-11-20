#include "common.h"


void func_8003D1D0(s32 arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;

    if ((((struct gd_1FC_s32 *)arg0)->v & 0x80000000) != 0) {
        if (((struct gd_204_s32 *)arg0)->v != 0) {
            func_800375A8(arg0, 1);
        } else {
            func_800375A8(arg0, 3);
        }
        sp10 = ((struct gd_AC_f32 *)arg0)->v;
        sp14 = ((struct gd_B4_f32 *)arg0)->v;
        sp18 = -((struct gd_B0_f32 *)arg0)->v;
        if (((f64)sp14) > D_8007B128) {
            ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & 0xFFFEFFFF;
        }
    }
}
