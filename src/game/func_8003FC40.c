#include "common.h"


void func_8003FC40(s32 arg0, s32 arg1, register s32 arg2) {
    u8 sp10;
    s32 sp14;
    s32 sp18;
    f32 sp1C;
    s32 pad0[2];
    s32 sp28;
    s32 pad1;
    s32 pad2[4];
    s32 sp40;
    f32 sp44;
    s32 sp48;
    s32 pad3;
    f32 sp50;
    f32 sp54;
    f32 sp58;
    s32 pad4;
    f32 sp60;
    f32 sp64;
    f32 sp68;
    s32 pad5;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    s32 pad6;
    s32 pad7[22];
    f64 spD8;

    sp10 = arg2;
    sp40 = 0;
    sp44 = 1.0f;
    sp48 = 0;
    sp18 = func_80042080(424);
    func_80069C70(sp18, 0, 424);
    ((struct gd_8_s16 *)sp18)->v = 21;
    ((struct gd_A_s16 *)sp18)->v = arg1;
    ((struct gd_15C_s32 *)sp18)->v = D_80133FB0;
    ((struct gd_160_s32 *)sp18)->v = 4;
    sp14 = *(s32 *)((((struct gd_A_u16 *)sp18)->v * 4) + ((s32)&D_800774B0));
    if (((struct gd_20_s32 *)sp14)->v != 0) {
        sp28 = func_80042080(((struct gd_20_s32 *)sp14)->v);
        ((struct gd_10_s32 *)sp14)->v = sp28;
        ((struct gd_18_s32 *)sp14)->v = sp28 + (((struct gd_4_u16 *)sp14)->v << 4);
        func_80069C70(((struct gd_18_s32 *)sp14)->v, 0, ((struct gd_20_s32 *)sp14)->v);
        func_80000EDC(((struct gd_10_s32 *)sp14)->v, ((struct gd_C_s32 *)sp14)->v, (((struct gd_4_u16 *)sp14)->v * 4));
    }
    ((struct gd_10_s32 *)sp18)->v = (s32)&D_80040338;
    ((struct gd_14_s32 *)sp18)->v = (s32)&D_80040898;
    ((struct gd_154_s32 *)sp18)->v = 0;
    ((struct gd_118_f32 *)sp18)->v = 0.1f;
    ((struct gd_11C_f32 *)sp18)->v = 0.1f;
    ((struct gd_120_f32 *)sp18)->v = 0.1f;
    ((struct gd_1A0_s32 *)sp18)->v = 0;
    ((struct gd_198_f32 *)sp18)->v = 1e+01f;
    ((struct gd_18C_f32 *)sp18)->v = ((struct gd_28_f32 *)arg0)->v;
    ((struct gd_128_s32 *)sp18)->v = 0;
    ((struct gd_124_s32 *)sp18)->v = 0;
    ((struct gd_144_s8 *)sp18)->v = 0;
    ((struct gd_145_s8 *)sp18)->v = sp10;
    ((struct gd_158_s32 *)sp18)->v = sp14;
    ((struct gd_24_f32 *)sp18)->v = ((struct gd_24_f32 *)arg0)->v;
    ((struct gd_28_f32 *)sp18)->v = ((struct gd_28_f32 *)arg0)->v;
    ((struct gd_2C_f32 *)sp18)->v = ((struct gd_2C_f32 *)arg0)->v;
    ((struct gd_30_f32 *)sp18)->v = ((struct gd_30_f32 *)arg0)->v;
    ((struct gd_34_f32 *)sp18)->v = ((struct gd_34_f32 *)arg0)->v;
    ((struct gd_38_f32 *)sp18)->v = ((struct gd_38_f32 *)arg0)->v;
    sp1C = sqrtf((((((struct gd_30_f32 *)sp18)->v * ((struct gd_30_f32 *)sp18)->v) + (((struct gd_34_f32 *)sp18)->v * ((struct gd_34_f32 *)sp18)->v)) + (((struct gd_38_f32 *)sp18)->v * ((struct gd_38_f32 *)sp18)->v)));
    if (sp1C >= 1e-07f) {
        spD8 = D_8007B1C0 / ((f64)sp1C);
        ((struct gd_30_f32 *)sp18)->v = (f32)(((f64)((struct gd_30_f32 *)sp18)->v) * spD8);
        ((struct gd_34_f32 *)sp18)->v = (f32)(((f64)((struct gd_34_f32 *)sp18)->v) * spD8);
        ((struct gd_38_f32 *)sp18)->v = (f32)(((f64)((struct gd_38_f32 *)sp18)->v) * spD8);
    } else {
    }
    ((struct gd_3C_f32 *)sp18)->v = ((struct gd_3C_f32 *)arg0)->v;
    ((struct gd_40_f32 *)sp18)->v = ((struct gd_40_f32 *)arg0)->v;
    ((struct gd_44_f32 *)sp18)->v = ((struct gd_44_f32 *)arg0)->v;
    sp1C = sqrtf((((((struct gd_3C_f32 *)sp18)->v * ((struct gd_3C_f32 *)sp18)->v) + (((struct gd_40_f32 *)sp18)->v * ((struct gd_40_f32 *)sp18)->v)) + (((struct gd_44_f32 *)sp18)->v * ((struct gd_44_f32 *)sp18)->v)));
    if (sp1C >= 1e-07f) {
        spD8 = D_8007B1C8 / ((f64)sp1C);
        ((struct gd_3C_f32 *)sp18)->v = (f32)(((f64)((struct gd_3C_f32 *)sp18)->v) * spD8);
        ((struct gd_40_f32 *)sp18)->v = (f32)(((f64)((struct gd_40_f32 *)sp18)->v) * spD8);
        ((struct gd_44_f32 *)sp18)->v = (f32)(((f64)((struct gd_44_f32 *)sp18)->v) * spD8);
    } else {
    }
    sp60 = 8.9f;
    sp64 = 84.2f;
    sp68 = -208.3f;
    sp50 = sp60;
    sp54 = sp64;
    sp58 = sp68;
    sp70 = *(f32 *)(sp18 + 36);
    sp74 = ((struct gd_4_f32 *)(sp18 + 36))->v;
    sp78 = ((struct gd_8_f32 *)(sp18 + 36))->v;
    sp50 = sp50 - sp70;
    sp54 = sp54 - sp74;
    sp58 = sp58 - sp78;
    spD8 = (f64)sqrtf((((sp50 * sp50) + (sp54 * sp54)) + (sp58 * sp58)));
    if (!(spD8 == 0.0)) {
        sp50 = (f32)(((f64)sp50) * (D_8007B1D0 / spD8));
        sp54 = (f32)(((f64)sp54) * (D_8007B1D8 / spD8));
        sp58 = (f32)(((f64)sp58) * (D_8007B1E0 / spD8));
    }
    (*(f32 *)(sp18 + 48)) = sp50;
    ((struct gd_4_f32 *)(sp18 + 48))->v = sp54;
    ((struct gd_8_f32 *)(sp18 + 48))->v = sp58;
    ((struct gd_48_f32 *)sp18)->v = (((struct gd_34_f32 *)sp18)->v * ((struct gd_44_f32 *)sp18)->v) - (((struct gd_38_f32 *)sp18)->v * ((struct gd_40_f32 *)sp18)->v);
    ((struct gd_4C_f32 *)sp18)->v = (((struct gd_38_f32 *)sp18)->v * ((struct gd_3C_f32 *)sp18)->v) - (((struct gd_30_f32 *)sp18)->v * ((struct gd_44_f32 *)sp18)->v);
    ((struct gd_50_f32 *)sp18)->v = (((struct gd_30_f32 *)sp18)->v * ((struct gd_40_f32 *)sp18)->v) - (((struct gd_34_f32 *)sp18)->v * ((struct gd_3C_f32 *)sp18)->v);
    ((struct gd_134_s32 *)sp18)->v = 0;
    func_80041A80(sp18, D_80115408, 4, 0);
    return;
}
