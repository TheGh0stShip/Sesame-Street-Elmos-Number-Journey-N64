struct gd_2_u16 { u8 pad[0x2]; u16 v; };
struct gd_4_u16 { u8 pad[0x4]; u16 v; };
struct gd_6_u16 { u8 pad[0x6]; u16 v; };
struct gd_8_s16 { u8 pad[0x8]; s16 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_A_s16 { u8 pad[0xA]; s16 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
struct gd_54_f32 { u8 pad[0x54]; f32 v; };
struct gd_54_s32 { u8 pad[0x54]; s32 v; };
struct gd_58_f32 { u8 pad[0x58]; f32 v; };
struct gd_5C_f32 { u8 pad[0x5C]; f32 v; };
struct gd_60_f32 { u8 pad[0x60]; f32 v; };
struct gd_EC_s32 { u8 pad[0xEC]; s32 v; };
struct gd_F0_s32 { u8 pad[0xF0]; s32 v; };
struct gd_1B8_s8 { u8 pad[0x1B8]; s8 v; };
struct gd_1B8_u8 { u8 pad[0x1B8]; u8 v; };
struct gd_1BC_s32 { u8 pad[0x1BC]; s32 v; };
struct gd_1C0_s32 { u8 pad[0x1C0]; s32 v; };
struct gd_1C4_f32 { u8 pad[0x1C4]; f32 v; };
struct gd_1C8_s32 { u8 pad[0x1C8]; s32 v; };
struct gd_1CC_s32 { u8 pad[0x1CC]; s32 v; };
struct gd_1D0_f32 { u8 pad[0x1D0]; f32 v; };
struct gd_1D8_s32 { u8 pad[0x1D8]; s32 v; };
struct gd_1E8_s32 { u8 pad[0x1E8]; s32 v; };
struct gd_1EC_s32 { u8 pad[0x1EC]; s32 v; };
struct gd_1F0_s32 { u8 pad[0x1F0]; s32 v; };
extern s32 D_800356F0;
extern s32 D_8003669C;
extern s32 D_8007693C;
extern s32 D_80076960;
extern s32 D_80076984;
extern s32 D_8007AE30;
extern s32 D_8007AE34;
extern f64 D_8007AE48;
extern f64 D_8007AE50;
extern u8 D_80106C70;
extern s32 D_80115408;
extern s32 D_80133FB0;
s32 func_80000EDC();
s32 func_8001E74C();
s32 func_80021640();
s32 func_80041A80();
s32 func_80042080();
f32 func_80056690(f32);
s32 func_80059540();
f64 func_80069920();
s32 func_80069C70();

s32 f(s32 arg0, register s32 arg1, s32 arg2) {
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
    sp28 = func_80056690((((((struct gd_30_f32 *)sp24)->v * ((struct gd_30_f32 *)sp24)->v) + (((struct gd_34_f32 *)sp24)->v * ((struct gd_34_f32 *)sp24)->v)) + (((struct gd_38_f32 *)sp24)->v * ((struct gd_38_f32 *)sp24)->v)));
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
    sp28 = func_80056690((((((struct gd_3C_f32 *)sp24)->v * ((struct gd_3C_f32 *)sp24)->v) + (((struct gd_40_f32 *)sp24)->v * ((struct gd_40_f32 *)sp24)->v)) + (((struct gd_44_f32 *)sp24)->v * ((struct gd_44_f32 *)sp24)->v)));
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
