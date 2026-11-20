#include "common.h"


void func_80036A90(s32 arg0) {
    s32 sp10;
    s32 sp14;
    f32 sp18;
    s32 sp1C;
    s32 pad0;
    s32 sp24;
    s32 pad1[6];
    s32 sp40;
    f32 sp44;
    s32 sp48;
    s32 pad2;
    s32 pad3[12];
    f64 sp80;

    sp10 = arg0;
    sp40 = 0;
    sp44 = 1.0f;
    sp48 = 0;
    sp14 = func_80042080(100);
    func_80069C70(sp14, 0, 100);
    ((struct gd_8_s16 *)sp14)->v = ((struct gd_2_u16 *)sp10)->v;
    ((struct gd_A_s16 *)sp14)->v = ((struct gd_6_u16 *)sp10)->v;
    ((struct gd_C_s32 *)sp14)->v = ((struct gd_8_s32 *)sp10)->v;
    ((struct gd_5C_s32 *)sp14)->v = D_80133FB0;
    ((struct gd_60_s32 *)sp14)->v = 4;
    ((struct gd_10_s32 *)sp14)->v = (s32)&D_80036EB8;
    ((struct gd_14_s32 *)sp14)->v = (s32)&D_80037418;
    ((struct gd_24_f32 *)sp14)->v = ((struct gd_C_f32 *)sp10)->v;
    ((struct gd_28_f32 *)sp14)->v = ((struct gd_10_f32 *)sp10)->v;
    ((struct gd_2C_f32 *)sp14)->v = ((struct gd_14_f32 *)sp10)->v;
    ((struct gd_30_f32 *)sp14)->v = ((struct gd_18_f32 *)sp10)->v;
    ((struct gd_34_f32 *)sp14)->v = ((struct gd_1C_f32 *)sp10)->v;
    ((struct gd_38_f32 *)sp14)->v = ((struct gd_20_f32 *)sp10)->v;
    sp18 = sqrtf((((((struct gd_30_f32 *)sp14)->v * ((struct gd_30_f32 *)sp14)->v) + (((struct gd_34_f32 *)sp14)->v * ((struct gd_34_f32 *)sp14)->v)) + (((struct gd_38_f32 *)sp14)->v * ((struct gd_38_f32 *)sp14)->v)));
    if (sp18 >= 1e-07f) {
        sp80 = D_8007AF70 / ((f64)sp18);
        ((struct gd_30_f32 *)sp14)->v = (f32)(((f64)((struct gd_30_f32 *)sp14)->v) * sp80);
        ((struct gd_34_f32 *)sp14)->v = (f32)(((f64)((struct gd_34_f32 *)sp14)->v) * sp80);
        ((struct gd_38_f32 *)sp14)->v = (f32)(((f64)((struct gd_38_f32 *)sp14)->v) * sp80);
    } else {
    }
    ((struct gd_3C_f32 *)sp14)->v = ((struct gd_24_f32 *)sp10)->v;
    ((struct gd_40_f32 *)sp14)->v = ((struct gd_28_f32 *)sp10)->v;
    ((struct gd_44_f32 *)sp14)->v = ((struct gd_2C_f32 *)sp10)->v;
    sp18 = sqrtf((((((struct gd_3C_f32 *)sp14)->v * ((struct gd_3C_f32 *)sp14)->v) + (((struct gd_40_f32 *)sp14)->v * ((struct gd_40_f32 *)sp14)->v)) + (((struct gd_44_f32 *)sp14)->v * ((struct gd_44_f32 *)sp14)->v)));
    if (sp18 >= 1e-07f) {
        sp80 = D_8007AF78 / ((f64)sp18);
        ((struct gd_3C_f32 *)sp14)->v = (f32)(((f64)((struct gd_3C_f32 *)sp14)->v) * sp80);
        ((struct gd_40_f32 *)sp14)->v = (f32)(((f64)((struct gd_40_f32 *)sp14)->v) * sp80);
        ((struct gd_44_f32 *)sp14)->v = (f32)(((f64)((struct gd_44_f32 *)sp14)->v) * sp80);
    } else {
    }
    func_80041A80(sp14, D_80115408, 4, 0);
    sp24 = func_800217C4(10);
    if (sp24 == 5) {
        sp1C = 0;
        while (!(sp1C >= 20)) {
            (*(s8 *)(sp1C + ((s32)&D_80076B78))) = *(u8 *)(sp1C + ((s32)&D_80076B64));
            sp1C++;
        }
    } else {
        sp1C = 0;
        while (!(sp1C >= 20)) {
            (*(s8 *)(sp1C + ((s32)&D_80076B78))) = func_800217C4(26);
            sp1C++;
        }
    }
    return;
}
