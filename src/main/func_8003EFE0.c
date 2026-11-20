struct gd_4_u16 { u8 pad[0x4]; u16 v; };
struct gd_8_s16 { u8 pad[0x8]; s16 v; };
struct gd_A_s16 { u8 pad[0xA]; s16 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
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
struct gd_1A8_f32 { u8 pad[0x1A8]; f32 v; };
struct gd_1AC_f32 { u8 pad[0x1AC]; f32 v; };
struct gd_1B0_f32 { u8 pad[0x1B0]; f32 v; };
struct gd_1B4_s32 { u8 pad[0x1B4]; s32 v; };
struct gd_1B8_s32 { u8 pad[0x1B8]; s32 v; };
struct gd_1C4_s32 { u8 pad[0x1C4]; s32 v; };
struct gd_1D4_f32 { u8 pad[0x1D4]; f32 v; };
struct gd_1D4_s8 { u8 pad[0x1D4]; s8 v; };
struct gd_1D8_f32 { u8 pad[0x1D8]; f32 v; };
struct gd_1DC_f32 { u8 pad[0x1DC]; f32 v; };
struct gd_1E8_s32 { u8 pad[0x1E8]; s32 v; };
struct gd_1EC_s32 { u8 pad[0x1EC]; s32 v; };
struct gd_1F0_s32 { u8 pad[0x1F0]; s32 v; };
extern s32 D_8003F52C;
extern s32 D_8003FC00;
extern f64 D_8007B190;
extern f64 D_8007B198;
extern s32 D_80115408;
extern s32 D_80133FB0;
s32 func_80000EDC();
s32 func_8001E74C();
s32 func_80041A80();
s32 func_80042080();
f32 func_80056690(f32);
s32 func_80069C70();

void f(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    f32 sp24;
    s32 pad0[2];
    s32 sp30;
    s32 pad1;
    s32 pad2[4];
    f32 sp48;
    f32 sp4C;
    f32 sp50;
    s32 pad3;
    s32 pad4[6];
    f64 sp70;

    sp18 = 0;
    sp1C = 0;
    sp48 = 0;
    sp4C = 1.0f;
    sp50 = 0;
    sp20 = func_80042080(536);
    func_80069C70(sp20, 0, 536);
    ((struct gd_8_s16 *)sp20)->v = 20;
    ((struct gd_A_s16 *)sp20)->v = arg4;
    ((struct gd_1EC_s32 *)sp20)->v = D_80133FB0;
    ((struct gd_1F0_s32 *)sp20)->v = 5;
    sp18 = 0;
    if (sp18 != 0) {
        if (((struct gd_20_s32 *)sp18)->v != 0) {
            sp30 = func_80042080(((struct gd_20_s32 *)sp18)->v);
            ((struct gd_10_s32 *)sp18)->v = sp30;
            ((struct gd_18_s32 *)sp18)->v = sp30 + (((struct gd_4_u16 *)sp18)->v << 4);
            func_80069C70(((struct gd_18_s32 *)sp18)->v, 0, ((struct gd_20_s32 *)sp18)->v);
            func_80000EDC(((struct gd_10_s32 *)sp18)->v, ((struct gd_C_s32 *)sp18)->v, (((struct gd_4_u16 *)sp18)->v * 4));
        }
    } else {
        return;
    }
    ((struct gd_10_s32 *)sp20)->v = (s32)&D_8003F52C;
    ((struct gd_14_s32 *)sp20)->v = (s32)&D_8003FC00;
    ((struct gd_1A8_f32 *)sp20)->v = 1.0f;
    ((struct gd_1AC_f32 *)sp20)->v = 1.0f;
    ((struct gd_1B0_f32 *)sp20)->v = 1.0f;
    ((struct gd_1B8_s32 *)sp20)->v = 0;
    ((struct gd_1B4_s32 *)sp20)->v = 0;
    ((struct gd_1D4_s8 *)sp20)->v = 0;
    ((struct gd_1D4_f32 *)sp20)->v = arg1;
    ((struct gd_1D8_f32 *)sp20)->v = arg2;
    ((struct gd_1DC_f32 *)sp20)->v = arg3;
    ((struct gd_1E8_s32 *)sp20)->v = sp18;
    ((struct gd_24_f32 *)sp20)->v = ((struct gd_24_f32 *)arg0)->v;
    ((struct gd_28_f32 *)sp20)->v = ((struct gd_28_f32 *)arg0)->v;
    ((struct gd_2C_f32 *)sp20)->v = ((struct gd_2C_f32 *)arg0)->v;
    ((struct gd_30_f32 *)sp20)->v = ((struct gd_30_f32 *)arg0)->v;
    ((struct gd_34_f32 *)sp20)->v = ((struct gd_34_f32 *)arg0)->v;
    ((struct gd_38_f32 *)sp20)->v = ((struct gd_38_f32 *)arg0)->v;
    sp24 = func_80056690((((((struct gd_30_f32 *)sp20)->v * ((struct gd_30_f32 *)sp20)->v) + (((struct gd_34_f32 *)sp20)->v * ((struct gd_34_f32 *)sp20)->v)) + (((struct gd_38_f32 *)sp20)->v * ((struct gd_38_f32 *)sp20)->v)));
    if (sp24 >= 1e-07f) {
        sp70 = D_8007B190 / ((f64)sp24);
        ((struct gd_30_f32 *)sp20)->v = (f32)(((f64)((struct gd_30_f32 *)sp20)->v) * sp70);
        ((struct gd_34_f32 *)sp20)->v = (f32)(((f64)((struct gd_34_f32 *)sp20)->v) * sp70);
        ((struct gd_38_f32 *)sp20)->v = (f32)(((f64)((struct gd_38_f32 *)sp20)->v) * sp70);
    } else {
    }
    ((struct gd_3C_f32 *)sp20)->v = sp48;
    ((struct gd_40_f32 *)sp20)->v = sp4C;
    ((struct gd_44_f32 *)sp20)->v = sp50;
    sp24 = func_80056690((((((struct gd_3C_f32 *)sp20)->v * ((struct gd_3C_f32 *)sp20)->v) + (((struct gd_40_f32 *)sp20)->v * ((struct gd_40_f32 *)sp20)->v)) + (((struct gd_44_f32 *)sp20)->v * ((struct gd_44_f32 *)sp20)->v)));
    if (sp24 >= 1e-07f) {
        sp70 = D_8007B198 / ((f64)sp24);
        ((struct gd_3C_f32 *)sp20)->v = (f32)(((f64)((struct gd_3C_f32 *)sp20)->v) * sp70);
        ((struct gd_40_f32 *)sp20)->v = (f32)(((f64)((struct gd_40_f32 *)sp20)->v) * sp70);
        ((struct gd_44_f32 *)sp20)->v = (f32)(((f64)((struct gd_44_f32 *)sp20)->v) * sp70);
    } else {
    }
    ((struct gd_48_f32 *)sp20)->v = (((struct gd_34_f32 *)sp20)->v * ((struct gd_44_f32 *)sp20)->v) - (((struct gd_38_f32 *)sp20)->v * ((struct gd_40_f32 *)sp20)->v);
    ((struct gd_4C_f32 *)sp20)->v = (((struct gd_38_f32 *)sp20)->v * ((struct gd_3C_f32 *)sp20)->v) - (((struct gd_30_f32 *)sp20)->v * ((struct gd_44_f32 *)sp20)->v);
    ((struct gd_50_f32 *)sp20)->v = (((struct gd_30_f32 *)sp20)->v * ((struct gd_40_f32 *)sp20)->v) - (((struct gd_34_f32 *)sp20)->v * ((struct gd_3C_f32 *)sp20)->v);
    func_8001E74C((sp20 + 360), (sp20 + 36), (sp20 + 48), (sp20 + 60), (sp20 + 72));
    ((struct gd_1C4_s32 *)sp20)->v = 0;
    func_80041A80(sp20, D_80115408, 4, 0);
    return;
}
