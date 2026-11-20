#include "common.h"


void func_80043170(s32 arg0) {
    f32 sp10;
    s32 pad0;
    f32 sp18;
    f32 sp1C;
    f32 sp20;

    sp10 = ((struct gd_38_f32 *)arg0)->v;
    if (sp10 > 0.0f) {
        ((struct gd_4_f32 *)arg0)->v = ((struct gd_4_f32 *)arg0)->v + (sp10 * ((struct gd_1C_f32 *)arg0)->v);
        ((struct gd_8_f32 *)arg0)->v = ((struct gd_8_f32 *)arg0)->v + (sp10 * ((struct gd_20_f32 *)arg0)->v);
        ((struct gd_C_f32 *)arg0)->v = ((struct gd_C_f32 *)arg0)->v + (sp10 * ((struct gd_24_f32 *)arg0)->v);
    }
    sp18 = ((struct gd_10_f32 *)arg0)->v - ((struct gd_40_f32 *)arg0)->v;
    sp1C = ((struct gd_14_f32 *)arg0)->v - ((struct gd_44_f32 *)arg0)->v;
    sp20 = ((struct gd_18_f32 *)arg0)->v - ((struct gd_48_f32 *)arg0)->v;
    func_80044348(((s32)&sp18));
    ((struct gd_10_f32 *)arg0)->v = ((struct gd_40_f32 *)arg0)->v + (sp18 * (*(f32 *)arg0));
    ((struct gd_14_f32 *)arg0)->v = ((struct gd_44_f32 *)arg0)->v + (sp1C * (*(f32 *)arg0));
    ((struct gd_18_f32 *)arg0)->v = ((struct gd_48_f32 *)arg0)->v + (sp20 * (*(f32 *)arg0));
}
