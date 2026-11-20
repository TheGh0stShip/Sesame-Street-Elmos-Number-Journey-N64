#include "common.h"


s32 func_80034804(s32 arg0, register s32 arg1, s32 arg2) {
    u8 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    f32 sp28;
    s32 pad0;
    s32 pad1;
    s32 sp34;
    s32 pad2[38];
    s32 pad3;
    s32 spD4;
    f64 spD8;

    sp18 = arg1;
    sp1C = arg0;
    sp24 = func_80042080(536);
    func_80069C70(sp24, 0, 536);
    ((struct gd_58_f32 *)sp24)->v = 1.0f;
    ((struct gd_5C_f32 *)sp24)->v = 1.0f;
    ((struct gd_60_f32 *)sp24)->v = 1.0f;
    ((struct gd_8_s16 *)sp24)->v = ((struct gd_2_u16 *)sp1C)->v;
    ((struct gd_A_s16 *)sp24)->v = ((struct gd_6_u16 *)sp1C)->v;
    ((struct gd_C_s32 *)sp24)->v = ((struct gd_8_s32 *)sp1C)->v;
    ((struct gd_1EC_s32 *)sp24)->v = D_80133FB0;
    ((struct gd_EC_s32 *)sp24)->v = 0;
    ((struct gd_F0_s32 *)sp24)->v = 0;
    ((struct gd_1F0_s32 *)sp24)->v = 4;
    ((struct gd_1B8_s8 *)sp24)->v = sp18;
    ((struct gd_1C0_s32 *)sp24)->v = arg2;
    ((struct gd_1BC_s32 *)sp24)->v = 0;
    ((struct gd_1CC_s32 *)sp24)->v = 0;
    ((struct gd_1D0_f32 *)sp24)->v = -3e+01f;
    ((struct gd_1D8_s32 *)sp24)->v = 0;
    if (((u32)((struct gd_1B8_u8 *)sp24)->v < 49) || ((u32)((struct gd_1B8_u8 *)sp24)->v >= 58)) {
        func_80059540(((s32)&D_8007AE30), ((s32)&D_8007AE34), 813);
    }
    spD4 = func_80021640();
    if ((D_80106C70 != 0) && ((spD4 & 3) == 0)) {
        if ((spD4 & 4) == 0) {
            sp20 = *(s32 *)((((struct gd_1B8_u8 *)sp24)->v * 4) + ((s32)&D_80076960));
        } else {
            sp20 = *(s32 *)((((struct gd_1B8_u8 *)sp24)->v * 4) + ((s32)&D_80076984));
        }
    } else {
        sp20 = *(s32 *)((((struct gd_1B8_u8 *)sp24)->v * 4) + ((s32)&D_8007693C));
    }
    if ((spD4 & 8) == 0) {
        ((struct gd_1D8_s32 *)sp24)->v = 1;
    } else {
        ((struct gd_1D8_s32 *)sp24)->v = 2;
    }
    if (((struct gd_20_s32 *)sp20)->v != 0) {
        sp34 = func_80042080(((struct gd_20_s32 *)sp20)->v);
        ((struct gd_10_s32 *)sp20)->v = sp34;
        ((struct gd_18_s32 *)sp20)->v = sp34 + (((struct gd_4_u16 *)sp20)->v << 4);
        func_80069C70(((struct gd_18_s32 *)sp20)->v, 0, ((struct gd_20_s32 *)sp20)->v);
        func_80000EDC(((struct gd_10_s32 *)sp20)->v, ((struct gd_C_s32 *)sp20)->v, (((struct gd_4_u16 *)sp20)->v * 4));
    }
    ((struct gd_1E8_s32 *)sp24)->v = sp20;
    ((struct gd_10_s32 *)sp24)->v = (s32)&D_800356F0;
    ((struct gd_14_s32 *)sp24)->v = (s32)&D_8003669C;
    ((struct gd_24_f32 *)sp24)->v = ((struct gd_C_f32 *)sp1C)->v;
    ((struct gd_28_f32 *)sp24)->v = ((struct gd_10_f32 *)sp1C)->v;
    ((struct gd_2C_f32 *)sp24)->v = ((struct gd_14_f32 *)sp1C)->v;
    ((struct gd_30_f32 *)sp24)->v = ((struct gd_18_f32 *)sp1C)->v;
    ((struct gd_34_f32 *)sp24)->v = ((struct gd_1C_f32 *)sp1C)->v;
    ((struct gd_38_f32 *)sp24)->v = ((struct gd_20_f32 *)sp1C)->v;
    sp28 = sqrtf((((((struct gd_30_f32 *)sp24)->v * ((struct gd_30_f32 *)sp24)->v) + (((struct gd_34_f32 *)sp24)->v * ((struct gd_34_f32 *)sp24)->v)) + (((struct gd_38_f32 *)sp24)->v * ((struct gd_38_f32 *)sp24)->v)));
    if (sp28 >= 1e-07f) {
        spD8 = D_8007AE48 / ((f64)sp28);
        ((struct gd_30_f32 *)sp24)->v = (f32)(((f64)((struct gd_30_f32 *)sp24)->v) * spD8);
        ((struct gd_34_f32 *)sp24)->v = (f32)(((f64)((struct gd_34_f32 *)sp24)->v) * spD8);
        ((struct gd_38_f32 *)sp24)->v = (f32)(((f64)((struct gd_38_f32 *)sp24)->v) * spD8);
    } else {
    }
    ((struct gd_3C_f32 *)sp24)->v = ((struct gd_24_f32 *)sp1C)->v;
    ((struct gd_40_f32 *)sp24)->v = ((struct gd_28_f32 *)sp1C)->v;
    ((struct gd_44_f32 *)sp24)->v = ((struct gd_2C_f32 *)sp1C)->v;
    sp28 = sqrtf((((((struct gd_3C_f32 *)sp24)->v * ((struct gd_3C_f32 *)sp24)->v) + (((struct gd_40_f32 *)sp24)->v * ((struct gd_40_f32 *)sp24)->v)) + (((struct gd_44_f32 *)sp24)->v * ((struct gd_44_f32 *)sp24)->v)));
    if (sp28 >= 1e-07f) {
        spD8 = D_8007AE50 / ((f64)sp28);
        ((struct gd_3C_f32 *)sp24)->v = (f32)(((f64)((struct gd_3C_f32 *)sp24)->v) * spD8);
        ((struct gd_40_f32 *)sp24)->v = (f32)(((f64)((struct gd_40_f32 *)sp24)->v) * spD8);
        ((struct gd_44_f32 *)sp24)->v = (f32)(((f64)((struct gd_44_f32 *)sp24)->v) * spD8);
    } else {
    }
    ((struct gd_48_f32 *)sp24)->v = (((struct gd_34_f32 *)sp24)->v * ((struct gd_44_f32 *)sp24)->v) - (((struct gd_38_f32 *)sp24)->v * ((struct gd_40_f32 *)sp24)->v);
    ((struct gd_4C_f32 *)sp24)->v = (((struct gd_38_f32 *)sp24)->v * ((struct gd_3C_f32 *)sp24)->v) - (((struct gd_30_f32 *)sp24)->v * ((struct gd_44_f32 *)sp24)->v);
    ((struct gd_50_f32 *)sp24)->v = (((struct gd_30_f32 *)sp24)->v * ((struct gd_40_f32 *)sp24)->v) - (((struct gd_34_f32 *)sp24)->v * ((struct gd_3C_f32 *)sp24)->v);
    func_8001E74C((sp24 + 376), (sp1C + 12), (sp1C + 24), (sp1C + 36), (sp24 + 72));
    ((struct gd_1C4_f32 *)sp24)->v = (f32)func_80069920(((f64)((struct gd_38_f32 *)sp24)->v), ((f64)((struct gd_30_f32 *)sp24)->v));
    ((struct gd_1C8_s32 *)sp24)->v = 0;
    func_80041A80(sp24, D_80115408, 4, 0);
    if (D_80106C70 == 2) {
        ((struct gd_54_f32 *)sp24)->v = 6e+01f;
    } else {
        ((struct gd_54_s32 *)sp24)->v = 0;
    }
    return sp24;
}
