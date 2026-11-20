#include "common.h"


void func_8001F748(s32 arg0, f32 arg1) {
    f32 sp10;
    f64 sp18;

    ((struct gd_18_f32 *)arg0)->v = ((struct gd_18_f32 *)arg0)->v + (arg1 * ((struct gd_24_f32 *)arg0)->v);
    ((struct gd_1C_f32 *)arg0)->v = ((struct gd_1C_f32 *)arg0)->v + (arg1 * ((struct gd_28_f32 *)arg0)->v);
    ((struct gd_20_f32 *)arg0)->v = ((struct gd_20_f32 *)arg0)->v + (arg1 * ((struct gd_2C_f32 *)arg0)->v);
    sp10 = sqrtf((((((struct gd_18_f32 *)arg0)->v * ((struct gd_18_f32 *)arg0)->v) + (((struct gd_1C_f32 *)arg0)->v * ((struct gd_1C_f32 *)arg0)->v)) + (((struct gd_20_f32 *)arg0)->v * ((struct gd_20_f32 *)arg0)->v)));
    if (sp10 >= 1e-07f) {
        sp18 = D_8007A538 / ((f64)sp10);
        ((struct gd_18_f32 *)arg0)->v = (f32)(((f64)((struct gd_18_f32 *)arg0)->v) * sp18);
        ((struct gd_1C_f32 *)arg0)->v = (f32)(((f64)((struct gd_1C_f32 *)arg0)->v) * sp18);
        ((struct gd_20_f32 *)arg0)->v = (f32)(((f64)((struct gd_20_f32 *)arg0)->v) * sp18);
    } else {
    }
    ((struct gd_24_f32 *)arg0)->v = (((struct gd_10_f32 *)arg0)->v * ((struct gd_20_f32 *)arg0)->v) - (((struct gd_14_f32 *)arg0)->v * ((struct gd_1C_f32 *)arg0)->v);
    ((struct gd_28_f32 *)arg0)->v = (((struct gd_14_f32 *)arg0)->v * ((struct gd_18_f32 *)arg0)->v) - (((struct gd_C_f32 *)arg0)->v * ((struct gd_20_f32 *)arg0)->v);
    ((struct gd_2C_f32 *)arg0)->v = (((struct gd_C_f32 *)arg0)->v * ((struct gd_1C_f32 *)arg0)->v) - (((struct gd_10_f32 *)arg0)->v * ((struct gd_18_f32 *)arg0)->v);
    sp10 = sqrtf((((((struct gd_24_f32 *)arg0)->v * ((struct gd_24_f32 *)arg0)->v) + (((struct gd_28_f32 *)arg0)->v * ((struct gd_28_f32 *)arg0)->v)) + (((struct gd_2C_f32 *)arg0)->v * ((struct gd_2C_f32 *)arg0)->v)));
    if (sp10 >= 1e-07f) {
        sp18 = D_8007A540 / ((f64)sp10);
        ((struct gd_24_f32 *)arg0)->v = (f32)(((f64)((struct gd_24_f32 *)arg0)->v) * sp18);
        ((struct gd_28_f32 *)arg0)->v = (f32)(((f64)((struct gd_28_f32 *)arg0)->v) * sp18);
        ((struct gd_2C_f32 *)arg0)->v = (f32)(((f64)((struct gd_2C_f32 *)arg0)->v) * sp18);
    } else {
    }
    return;
}
