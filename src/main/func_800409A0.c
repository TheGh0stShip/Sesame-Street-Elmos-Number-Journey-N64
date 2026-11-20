struct gd_2_u16 { u8 pad[0x2]; u16 v; };
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
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
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
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
struct gd_1BC_s32 { u8 pad[0x1BC]; s32 v; };
struct gd_1C4_s32 { u8 pad[0x1C4]; s32 v; };
struct gd_1CC_s32 { u8 pad[0x1CC]; s32 v; };
struct gd_1D4_s32 { u8 pad[0x1D4]; s32 v; };
struct gd_1D8_s32 { u8 pad[0x1D8]; s32 v; };
struct gd_1DC_s32 { u8 pad[0x1DC]; s32 v; };
struct gd_1E0_s32 { u8 pad[0x1E0]; s32 v; };
struct gd_1E0_s8 { u8 pad[0x1E0]; s8 v; };
struct gd_1E8_s32 { u8 pad[0x1E8]; s32 v; };
extern s32 D_80040E34;
extern s32 D_80041848;
extern s32 D_80076800;
extern s32 D_8007B240;
extern s32 D_8007B244;
extern f64 D_8007B258;
extern f64 D_8007B260;
extern s32 D_80115408;
extern s32 D_8016DA50;
extern s32 D_80177388;
extern s32 D_801811D0;
s32 func_800189A0();
s32 func_80041A80();
s32 func_80042080();
f32 func_80056690(f32);
s32 func_80059540();
s32 func_80069C70();

void f(s32 arg0, s32 arg1) {
    register s32 reg_s0;
    register s32 reg_s1;
    register s32 reg_s2;
    s32 sp10;
    s32 sp14;
    f32 sp18;
    f64 sp20;

    sp10 = 0;
    sp14 = 0;
    reg_s1 = arg0;
    reg_s2 = arg0;
    reg_s0 = func_80042080(536);
    switch (reg_s0) {
    case 0:
        func_80059540(((s32)&D_8007B240), ((s32)&D_8007B244), 139);
    }
    if (reg_s0 == 0) {
        return;
    }
    func_80069C70(reg_s0, 0, 536);
    ((struct gd_10_s32 *)reg_s0)->v = (s32)&D_80040E34;
    ((struct gd_14_s32 *)reg_s0)->v = (s32)&D_80041848;
    ((struct gd_18_s32 *)reg_s0)->v = 0;
    ((struct gd_8_s16 *)reg_s0)->v = ((struct gd_2_u16 *)reg_s2)->v;
    ((struct gd_A_s16 *)reg_s0)->v = ((struct gd_6_u16 *)reg_s2)->v;
    ((struct gd_C_s32 *)reg_s0)->v = ((struct gd_8_s32 *)reg_s2)->v;
    sp14 = arg1;
    sp10 = func_800189A0(sp14, ((struct gd_4_u16 *)sp14)->v);
    switch (sp10) {
    case 0:
        func_80059540(((s32)&D_8007B240), ((s32)&D_8007B244), 157);
    }
    ((struct gd_1C4_s32 *)reg_s0)->v = 0;
    ((struct gd_1E8_s32 *)reg_s0)->v = sp10;
    ((struct gd_1BC_s32 *)reg_s0)->v = 0;
    ((struct gd_1D4_s32 *)reg_s0)->v = 0;
    ((struct gd_1D8_s32 *)reg_s0)->v = 0;
    ((struct gd_1DC_s32 *)reg_s0)->v = 0;
    ((struct gd_1E0_s32 *)reg_s0)->v = 0;
    switch (((struct gd_4_s32 *)reg_s2)->v) {
    case 0x5001:
        ((struct gd_30_f32 *)D_80076800)->v = ((struct gd_C_f32 *)reg_s2)->v;
        ((struct gd_34_f32 *)D_80076800)->v = ((struct gd_10_f32 *)reg_s2)->v;
        ((struct gd_38_f32 *)D_80076800)->v = ((struct gd_14_f32 *)reg_s2)->v;
        ((struct gd_34_s32 *)D_80076800)->v = 0;
        ((struct gd_1BC_s32 *)reg_s0)->v = (s32)&D_801811D0;
        break;
    case 0x5003:
        ((struct gd_1BC_s32 *)reg_s0)->v = (s32)&D_80177388;
        break;
    case 0x5005:
        ((struct gd_1BC_s32 *)reg_s0)->v = (s32)&D_8016DA50;
        break;
    case 0x6000:
        ((struct gd_1E0_s8 *)reg_s0)->v = 1;
        break;
    }
    ((struct gd_1CC_s32 *)reg_s0)->v = 0;
    ((struct gd_C_s32 *)reg_s0)->v = ((struct gd_C_s32 *)reg_s0)->v & -129;
    ((struct gd_1A8_f32 *)reg_s0)->v = 1.0f;
    ((struct gd_1AC_f32 *)reg_s0)->v = 1.0f;
    ((struct gd_1B0_f32 *)reg_s0)->v = 1.0f;
    ((struct gd_24_f32 *)reg_s0)->v = ((struct gd_C_f32 *)reg_s2)->v;
    ((struct gd_28_f32 *)reg_s0)->v = ((struct gd_10_f32 *)reg_s2)->v;
    ((struct gd_2C_f32 *)reg_s0)->v = ((struct gd_14_f32 *)reg_s2)->v;
    ((struct gd_30_f32 *)reg_s0)->v = ((struct gd_18_f32 *)reg_s2)->v;
    ((struct gd_34_f32 *)reg_s0)->v = ((struct gd_1C_f32 *)reg_s2)->v;
    ((struct gd_38_f32 *)reg_s0)->v = ((struct gd_20_f32 *)reg_s2)->v;
    sp18 = func_80056690((((((struct gd_30_f32 *)reg_s0)->v * ((struct gd_30_f32 *)reg_s0)->v) + (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_34_f32 *)reg_s0)->v)) + (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_38_f32 *)reg_s0)->v)));
    if (sp18 >= 1e-07f) {
        sp20 = D_8007B258 / ((f64)sp18);
        ((struct gd_30_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_30_f32 *)reg_s0)->v) * sp20);
        ((struct gd_34_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_34_f32 *)reg_s0)->v) * sp20);
        ((struct gd_38_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_38_f32 *)reg_s0)->v) * sp20);
    } else {
    }
    ((struct gd_3C_f32 *)reg_s0)->v = ((struct gd_24_f32 *)reg_s2)->v;
    ((struct gd_40_f32 *)reg_s0)->v = ((struct gd_28_f32 *)reg_s2)->v;
    ((struct gd_44_f32 *)reg_s0)->v = ((struct gd_2C_f32 *)reg_s2)->v;
    sp18 = func_80056690((((((struct gd_3C_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v) + (((struct gd_40_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v)) + (((struct gd_44_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v)));
    if (sp18 >= 1e-07f) {
        sp20 = D_8007B260 / ((f64)sp18);
        ((struct gd_3C_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_3C_f32 *)reg_s0)->v) * sp20);
        ((struct gd_40_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_40_f32 *)reg_s0)->v) * sp20);
        ((struct gd_44_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_44_f32 *)reg_s0)->v) * sp20);
    } else {
    }
    ((struct gd_48_f32 *)reg_s0)->v = (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v) - (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v);
    ((struct gd_4C_f32 *)reg_s0)->v = (((struct gd_38_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v) - (((struct gd_30_f32 *)reg_s0)->v * ((struct gd_44_f32 *)reg_s0)->v);
    ((struct gd_50_f32 *)reg_s0)->v = (((struct gd_30_f32 *)reg_s0)->v * ((struct gd_40_f32 *)reg_s0)->v) - (((struct gd_34_f32 *)reg_s0)->v * ((struct gd_3C_f32 *)reg_s0)->v);
    func_80041A80(reg_s0, D_80115408, 4, 0);
}
