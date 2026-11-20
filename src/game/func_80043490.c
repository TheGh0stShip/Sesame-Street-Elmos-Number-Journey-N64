#include "common.h"


void func_80043490(s32 arg0) {
    s32 sp10;
    s32 pad0;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    s32 pad1;
    f32 sp28;
    f32 sp2C;
    f32 sp30;
    s32 pad2;
    s32 pad3;
    f32 sp3C;

    sp10 = ((struct gd_3C_s32 *)arg0)->v;
    if (((struct gd_38_f32 *)arg0)->v > 0.0f) {
        ((struct gd_4_f32 *)arg0)->v = ((struct gd_4_f32 *)arg0)->v + (((struct gd_1C_f32 *)arg0)->v * ((struct gd_38_f32 *)arg0)->v);
        ((struct gd_8_f32 *)arg0)->v = ((struct gd_8_f32 *)arg0)->v + (((struct gd_20_f32 *)arg0)->v * ((struct gd_38_f32 *)arg0)->v);
        ((struct gd_C_f32 *)arg0)->v = ((struct gd_C_f32 *)arg0)->v + (((struct gd_24_f32 *)arg0)->v * ((struct gd_38_f32 *)arg0)->v);
    }
    switch (*(s32 *)sp10) {
    case 2:
        sp28 = ((struct gd_10_f32 *)sp10)->v;
        sp2C = ((struct gd_14_f32 *)sp10)->v;
        sp30 = ((struct gd_18_f32 *)sp10)->v;
        break;
    case 3:
        sp28 = ((struct gd_10_f32 *)sp10)->v;
        sp2C = ((struct gd_14_f32 *)sp10)->v;
        sp30 = ((struct gd_18_f32 *)sp10)->v;
        break;
    case 10:
        sp28 = ((struct gd_10_f32 *)sp10)->v;
        sp2C = ((struct gd_14_f32 *)sp10)->v;
        sp30 = ((struct gd_18_f32 *)sp10)->v;
        break;
    case 4:
    case 12:
        sp28 = ((struct gd_4C_f32 *)arg0)->v;
        sp2C = ((struct gd_50_f32 *)arg0)->v;
        sp30 = ((struct gd_54_f32 *)arg0)->v;
        break;
    default:
        func_80059540(((s32)&D_8007B2A0), ((s32)&D_8007B2A4), 125);
        break;
    }
    sp18 = -((struct gd_1C_f32 *)arg0)->v;
    sp1C = -((struct gd_20_f32 *)arg0)->v;
    sp20 = -((struct gd_24_f32 *)arg0)->v;
    sp3C = ((sp18 * sp28) + (sp1C * sp2C)) + (sp20 * sp30);
    sp3C = sp3C * 2.0f;
    ((struct gd_1C_f32 *)arg0)->v = (sp28 * sp3C) - sp18;
    ((struct gd_20_f32 *)arg0)->v = (sp2C * sp3C) - sp1C;
    ((struct gd_24_f32 *)arg0)->v = (sp30 * sp3C) - sp20;
    ((struct gd_28_f32 *)arg0)->v = ((struct gd_28_f32 *)arg0)->v - ((struct gd_38_f32 *)arg0)->v;
    if (((struct gd_28_f32 *)arg0)->v > 0.0f) {
        ((struct gd_10_f32 *)arg0)->v = ((struct gd_4_f32 *)arg0)->v + (((struct gd_1C_f32 *)arg0)->v * ((struct gd_28_f32 *)arg0)->v);
        ((struct gd_14_f32 *)arg0)->v = ((struct gd_8_f32 *)arg0)->v + (((struct gd_20_f32 *)arg0)->v * ((struct gd_28_f32 *)arg0)->v);
        ((struct gd_18_f32 *)arg0)->v = ((struct gd_C_f32 *)arg0)->v + (((struct gd_24_f32 *)arg0)->v * ((struct gd_28_f32 *)arg0)->v);
    }
}
