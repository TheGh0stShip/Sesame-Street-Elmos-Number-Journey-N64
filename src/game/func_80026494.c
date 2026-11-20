#include "common.h"

void func_80026494(s32 arg0, s32 arg1, s32 arg2) {
    if ((*(f32 *)arg1) > 0.0f) {
        if (((struct gd_4_f32 *)arg1)->v > 0.0f) {
            if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
            } else {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
            }
        } else {
            if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
            } else {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
            }
        }
        return;
    }
    if (((struct gd_4_f32 *)arg1)->v > 0.0f) {
        if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
            (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
            ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
            ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
        } else {
            (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
            ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
            ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
        }
        return;
    }
    if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
        (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
        ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
        ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
        return;
    }
    (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
    ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
    ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
}
