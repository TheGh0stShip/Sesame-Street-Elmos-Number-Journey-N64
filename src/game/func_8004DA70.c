#include "common.h"


void func_8004DA70(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp10;
    s32 sp14;
    f32 sp18;

    if (arg0 == 0) {
        return;
    }
    sp10 = arg0;
    sp14 = *(s32 *)sp10;
    memcpy(arg2, arg1, 40);
    if ((((struct gd_C_s32 *)sp10)->v == 0) || (((struct gd_C_s32 *)sp10)->v >= 13)) {
        if (((struct gd_8_s32 *)sp10)->v < 0) {
            if (((struct gd_4_s32 *)sp10)->v > ((struct gd_8_s32 *)sp10)->v) {
                if (((struct gd_4_s32 *)sp10)->v < (-((struct gd_8_s32 *)sp10)->v)) {
                    if (((struct gd_18_f32 *)arg2)->v < 4.3e+02f) {
                        ((struct gd_C_s32 *)sp10)->v = 1;
                    }
                }
            }
        }
    } else {
        if (((struct gd_18_f32 *)arg2)->v > 5e+02f) {
            ((struct gd_C_s32 *)sp10)->v = 13;
        }
    }
    if (((struct gd_C_s32 *)sp10)->v > 0) {
        ((struct gd_24_s32 *)arg2)->v = ((struct gd_24_s32 *)arg2)->v | 32;
        if (((struct gd_C_s32 *)sp10)->v < 13) {
            ((struct gd_C_s32 *)sp10)->v++;
            ((struct gd_C_f32 *)arg2)->v = (f32)(((f64)((struct gd_C_f32 *)arg2)->v) + D_8007B4E8);
        }
        if (((struct gd_C_s32 *)sp10)->v >= 13) {
            ((struct gd_20_f32 *)arg2)->v = ((struct gd_20_f32 *)arg2)->v + 0.12f;
            sp18 = func_8004B828(((struct gd_FC_f32 *)sp14)->v, ((struct gd_C_f32 *)arg2)->v);
            if (sp18 > -0.12f) {
                if (sp18 < 0.12f) {
                    ((struct gd_C_s32 *)sp10)->v = 0;
                }
            }
        }
    }
}
