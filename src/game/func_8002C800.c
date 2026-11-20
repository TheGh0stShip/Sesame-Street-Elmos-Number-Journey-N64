#include "common.h"

extern s32 D_80106C84;

void func_8002C800(s32 arg0) {
    register s32 reg_s0;
    register s32 reg_s1;
    s32 sp10;
    s32 sp14;
    f32 sp18;
    s32 pad0;
    s32 pad1[2];
    s32 sp28;
    s32 pad2;
    s32 pad3[12];
    f64 sp60;

    sp14 = 0;
    sp28 = 0;
    sp10 = arg0;
    reg_s1 = arg0;
    switch (D_8012F154) {
    case 10:
        sp28 = (s32)&D_801AA700;
        break;
    case 5:
        sp28 = (s32)&D_801BFF98;
        break;
    case 6:
        sp28 = (s32)&D_80187430;
        break;
    default:
        sp28 = (s32)&D_8027B9D0;
        break;
    }
    sp14 = func_800189A0(sp28, ((struct gd_4_u16 *)sp28)->v);
    switch (sp14) {
    case 0:
        func_80059540(((s32)&D_8007A960), ((s32)&D_8007A964), 400);
    }
    if ((*(s32 *)reg_s1) == 5) {
        goto L58;
    }
    func_80059540(((s32)&D_8007A960), ((s32)&D_8007A964), 403);
L58: ;
    reg_s0 = func_80042080(560);
    switch (reg_s0) {
    case 0:
        func_80059540(((s32)&D_8007A960), ((s32)&D_8007A964), 406);
    }
    if (reg_s0 == 0) {
        return;
    }
    D_80106C84 = reg_s0;
    func_80069C70(reg_s0, 0, 560);
    ((struct gd_10_s32 *)reg_s0)->v = (s32)&D_8002D0A4;
    ((struct gd_14_s32 *)reg_s0)->v = (s32)&D_8002E308;
    ((struct gd_18_s32 *)reg_s0)->v = (s32)&D_80032670;
    ((struct gd_8_s16 *)reg_s0)->v = ((struct gd_2_u16 *)reg_s1)->v;
    ((struct gd_A_s16 *)reg_s0)->v = ((struct gd_6_u16 *)reg_s1)->v;
    ((struct gd_C_s32 *)reg_s0)->v = ((struct gd_8_s32 *)reg_s1)->v;
    ((struct gd_54_f32 *)reg_s0)->v = 1.0f;
    ((struct gd_58_f32 *)reg_s0)->v = 1.0f;
    ((struct gd_5C_f32 *)reg_s0)->v = 1.0f;
    ((struct gd_F0_s32 *)reg_s0)->v = sp14;
    ((struct gd_24_f32 *)reg_s0)->v = ((struct gd_C_f32 *)reg_s1)->v;
    ((struct gd_28_f32 *)reg_s0)->v = ((struct gd_10_f32 *)reg_s1)->v;
    ((struct gd_2C_f32 *)reg_s0)->v = ((struct gd_14_f32 *)reg_s1)->v;
    ((struct gd_30_f32 *)reg_s0)->v = ((struct gd_18_f32 *)reg_s1)->v;
    ((struct gd_34_f32 *)reg_s0)->v = ((struct gd_1C_f32 *)reg_s1)->v;
    ((struct gd_38_f32 *)reg_s0)->v = ((struct gd_20_f32 *)reg_s1)->v;
    sp18 = sqrtf((((((struct gd_30_f32 *)reg_s0)->v * ((struct gd_30_f32 *)reg_s0)->v) + (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_34_f32 *)reg_s0)->v)) + (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_38_f32 *)reg_s0)->v)));
    if (sp18 >= 1e-07f) {
        sp60 = D_8007A978 / ((f64)sp18);
        ((struct gd_30_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_30_f32 *)reg_s0)->v) * sp60);
        ((struct gd_34_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_34_f32 *)reg_s0)->v) * sp60);
        ((struct gd_38_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_38_f32 *)reg_s0)->v) * sp60);
    } else {
    }
    ((struct gd_1B8_f32 *)reg_s0)->v = ((struct gd_30_f32 *)reg_s0)->v;
    ((struct gd_1BC_f32 *)reg_s0)->v = ((struct gd_34_f32 *)reg_s0)->v;
    ((struct gd_1C0_f32 *)reg_s0)->v = ((struct gd_38_f32 *)reg_s0)->v;
    D_8012F144 = ((struct gd_30_f32 *)reg_s0)->v;
    D_8012F148 = ((struct gd_34_f32 *)reg_s0)->v;
    D_8012F14C = ((struct gd_38_f32 *)reg_s0)->v;
    ((struct gd_3C_f32 *)reg_s0)->v = ((struct gd_24_f32 *)reg_s1)->v;
    ((struct gd_40_f32 *)reg_s0)->v = ((struct gd_28_f32 *)reg_s1)->v;
    ((struct gd_44_f32 *)reg_s0)->v = ((struct gd_2C_f32 *)reg_s1)->v;
    sp18 = sqrtf((((((struct gd_3C_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v) + (((struct gd_40_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v)) + (((struct gd_44_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v)));
    if (sp18 >= 1e-07f) {
        sp60 = D_8007A980 / ((f64)sp18);
        ((struct gd_3C_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_3C_f32 *)reg_s0)->v) * sp60);
        ((struct gd_40_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_40_f32 *)reg_s0)->v) * sp60);
        ((struct gd_44_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_44_f32 *)reg_s0)->v) * sp60);
    } else {
    }
    ((struct gd_48_f32 *)reg_s0)->v = (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v) - (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v);
    ((struct gd_4C_f32 *)reg_s0)->v = (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v) - (((struct gd_30_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v);
    ((struct gd_50_f32 *)reg_s0)->v = (((struct gd_30_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v) - (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v);
    ((struct gd_1F8_s32 *)reg_s0)->v = 0;
    ((struct gd_1F4_f32 *)reg_s0)->v = -3.0f;
    ((struct gd_204_s32 *)reg_s0)->v = 0;
    ((struct gd_1D4_s32 *)reg_s0)->v = 2;
    ((struct gd_226_s16 *)reg_s0)->v = 8;
    ((struct gd_1D8_s32 *)reg_s0)->v = 0;
    ((struct gd_1DC_s32 *)reg_s0)->v = 0;
    ((struct gd_1E0_s32 *)reg_s0)->v = 0;
    ((struct gd_1E4_s32 *)reg_s0)->v = 0;
    ((struct gd_1E8_s32 *)reg_s0)->v = 0;
    ((struct gd_1EC_s32 *)reg_s0)->v = 0;
    ((struct gd_1F0_s32 *)reg_s0)->v = 0;
    ((struct gd_200_s32 *)reg_s0)->v = 0;
    ((struct gd_214_s32 *)reg_s0)->v = 0;
    ((struct gd_1AC_s32 *)reg_s0)->v = 0;
    ((struct gd_1B0_s32 *)reg_s0)->v = 0;
    ((struct gd_1B4_s32 *)reg_s0)->v = 0;
    D_800A9148 = 0;
    switch (D_8012F154) {
    case 1:
        ((struct gd_1F4_f32 *)reg_s0)->v = -2.0f;
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8002E82C;
        ((struct gd_60_f32 *)reg_s0)->v = 75.0f;
        func_800375A8(reg_s0, 7);
        break;
    case 9:
        ((struct gd_1F4_f32 *)reg_s0)->v = -2.0f;
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8002E82C;
        ((struct gd_60_f32 *)reg_s0)->v = 75.0f;
        func_800375A8(reg_s0, 10);
        break;
    case 10:
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8003086C;
        ((struct gd_60_f32 *)reg_s0)->v = 175.0f;
        ((struct gd_1C4_f32 *)reg_s0)->v = 4e+01f;
        func_800375A8(reg_s0, 12);
        break;
    case 7:
        ((struct gd_1F4_f32 *)reg_s0)->v = -2.0f;
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8002E82C;
        ((struct gd_60_f32 *)reg_s0)->v = 75.0f;
        func_800375A8(reg_s0, 16);
        break;
    case 8:
        ((struct gd_1F4_f32 *)reg_s0)->v = -5.0f;
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8002E82C;
        ((struct gd_60_f32 *)reg_s0)->v = 75.0f;
        func_800375A8(reg_s0, 18);
        break;
    case 5:
        ((struct gd_1F4_f32 *)reg_s0)->v = -2.0f;
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8003086C;
        ((struct gd_60_f32 *)reg_s0)->v = 125.0f;
        ((struct gd_1C4_f32 *)reg_s0)->v = 5e+01f;
        func_800375A8(reg_s0, 20);
        break;
    case 6:
        ((struct gd_1F4_f32 *)reg_s0)->v = -0.5f;
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8003086C;
        ((struct gd_60_f32 *)reg_s0)->v = 1e+02f;
        ((struct gd_1C4_f32 *)reg_s0)->v = 6e+01f;
        func_800375A8(reg_s0, 25);
        break;
    case 11:
        D_80107CF0 = 0;
        D_8012FDE4 = 0;
        ((struct gd_EC_s32 *)reg_s0)->v = 0;
        ((struct gd_1F0_s8 *)reg_s0)->v = 1;
        ((struct gd_28_f32 *)reg_s0)->v = 75.0f;
        ((struct gd_30_s32 *)reg_s0)->v = 0;
        ((struct gd_34_s32 *)reg_s0)->v = 0;
        ((struct gd_38_f32 *)reg_s0)->v = 1.0f;
        ((struct gd_60_f32 *)reg_s0)->v = 75.0f;
        func_800375A8(reg_s0, 30);
        ((struct gd_48_f32 *)reg_s0)->v = (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v) - (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v);
        ((struct gd_4C_f32 *)reg_s0)->v = (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v) - (((struct gd_30_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v);
        ((struct gd_50_f32 *)reg_s0)->v = (((struct gd_30_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v) - (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v);
        break;
    default:
        ((struct gd_EC_s32 *)reg_s0)->v = (s32)&D_8002E82C;
        ((struct gd_60_f32 *)reg_s0)->v = 75.0f;
        func_800375A8(reg_s0, 7);
        break;
    }
    func_80041A80(reg_s0, D_80115408, 4, 0);
    ((struct gd_B8_s32 *)reg_s0)->v = ((struct gd_30_s32 *)reg_s1)->v;
    ((struct gd_8C_s32 *)reg_s0)->v = 0;
    ((struct gd_90_s32 *)reg_s0)->v = -2;
    ((struct gd_BC_s32 *)reg_s0)->v = 0;
    ((struct gd_C0_s32 *)reg_s0)->v = 0;
    ((struct gd_C4_s32 *)reg_s0)->v = reg_s0 + 200;
    ((struct gd_C8_s32 *)reg_s0)->v = 1;
    ((struct gd_CC_s32 *)reg_s0)->v = reg_s0;
    ((struct gd_D0_s32 *)reg_s0)->v = 1;
    ((struct gd_E4_f32 *)reg_s0)->v = ((struct gd_60_f32 *)reg_s0)->v;
    ((struct gd_D8_f32 *)reg_s0)->v = ((struct gd_24_f32 *)reg_s0)->v;
    ((struct gd_DC_f32 *)reg_s0)->v = -((struct gd_2C_f32 *)reg_s0)->v;
    ((struct gd_E0_f32 *)reg_s0)->v = ((struct gd_28_f32 *)reg_s0)->v;
    func_80046250(((struct gd_B8_s32 *)reg_s0)->v, (reg_s0 + 188), 2, 0);
    func_80017C00(reg_s0, 0);
    ((struct gd_228_s16 *)reg_s0)->v = 0xFFFF8000;
    D_80106C60 = 0;
    (*(f32 *)&D_80106C60) = 1.0f;
    D_80106C60 = 0;
}
