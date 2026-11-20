#include "common.h"

void func_80043AF8(s32 arg0) {
    f32 sp0;

    sp0 = ((struct gd_38_f32 *)arg0)->v;
    if (sp0 > 0.0f) {
        ((struct gd_4_f32 *)arg0)->v = ((struct gd_4_f32 *)arg0)->v + (sp0 * ((struct gd_1C_f32 *)arg0)->v);
        ((struct gd_8_f32 *)arg0)->v = ((struct gd_8_f32 *)arg0)->v + (sp0 * ((struct gd_20_f32 *)arg0)->v);
        ((struct gd_C_f32 *)arg0)->v = ((struct gd_C_f32 *)arg0)->v + (sp0 * ((struct gd_24_f32 *)arg0)->v);
    }
    ((struct gd_10_f32 *)arg0)->v = ((struct gd_40_f32 *)arg0)->v;
    ((struct gd_14_f32 *)arg0)->v = ((struct gd_44_f32 *)arg0)->v;
    ((struct gd_18_f32 *)arg0)->v = ((struct gd_48_f32 *)arg0)->v;
}
