#include "common.h"

void func_800432EC(s32 arg0) {
    s32 sp0;
    f32 sp4;

    sp0 = ((struct gd_3C_s32 *)arg0)->v;
    sp4 = ((struct gd_38_f32 *)arg0)->v;
    if (sp4 > 0.0f) {
        ((struct gd_4_f32 *)arg0)->v = ((struct gd_4_f32 *)arg0)->v + (sp4 * ((struct gd_1C_f32 *)arg0)->v);
        ((struct gd_8_f32 *)arg0)->v = ((struct gd_8_f32 *)arg0)->v + (sp4 * ((struct gd_20_f32 *)arg0)->v);
        ((struct gd_C_f32 *)arg0)->v = ((struct gd_C_f32 *)arg0)->v + (sp4 * ((struct gd_24_f32 *)arg0)->v);
    }
    sp4 = (((struct gd_1C_f32 *)sp0)->v + (*(f32 *)arg0)) - ((((*(f32 *)(sp0 + 16)) * (*(f32 *)(arg0 + 16))) + (((struct gd_4_f32 *)(sp0 + 16))->v * ((struct gd_4_f32 *)(arg0 + 16))->v)) + (((struct gd_8_f32 *)(sp0 + 16))->v * ((struct gd_8_f32 *)(arg0 + 16))->v));
    ((struct gd_10_f32 *)arg0)->v = ((struct gd_10_f32 *)arg0)->v + (sp4 * ((struct gd_10_f32 *)sp0)->v);
    ((struct gd_14_f32 *)arg0)->v = ((struct gd_14_f32 *)arg0)->v + (sp4 * ((struct gd_14_f32 *)sp0)->v);
    ((struct gd_18_f32 *)arg0)->v = ((struct gd_18_f32 *)arg0)->v + (sp4 * ((struct gd_18_f32 *)sp0)->v);
}
