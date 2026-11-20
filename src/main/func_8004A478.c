struct gd_60_f32 { u8 pad[0x60]; f32 v; };
struct gd_64_f32 { u8 pad[0x64]; f32 v; };
struct gd_68_f32 { u8 pad[0x68]; f32 v; };
struct gd_6C_f32 { u8 pad[0x6C]; f32 v; };
struct gd_70_f32 { u8 pad[0x70]; f32 v; };
struct gd_74_f32 { u8 pad[0x74]; f32 v; };
struct gd_78_f32 { u8 pad[0x78]; f32 v; };
struct gd_7C_f32 { u8 pad[0x7C]; f32 v; };
struct gd_80_f32 { u8 pad[0x80]; f32 v; };
struct gd_84_f32 { u8 pad[0x84]; f32 v; };
struct gd_88_f32 { u8 pad[0x88]; f32 v; };
struct gd_8C_f32 { u8 pad[0x8C]; f32 v; };
struct gd_90_f32 { u8 pad[0x90]; f32 v; };
struct gd_94_f32 { u8 pad[0x94]; f32 v; };
struct gd_98_f32 { u8 pad[0x98]; f32 v; };
struct gd_9C_f32 { u8 pad[0x9C]; f32 v; };
struct gd_A0_f32 { u8 pad[0xA0]; f32 v; };
struct gd_A4_f32 { u8 pad[0xA4]; f32 v; };
struct gd_A8_f32 { u8 pad[0xA8]; f32 v; };
struct gd_AC_f32 { u8 pad[0xAC]; f32 v; };
struct gd_B0_f32 { u8 pad[0xB0]; f32 v; };
struct gd_B4_f32 { u8 pad[0xB4]; f32 v; };
struct gd_B8_f32 { u8 pad[0xB8]; f32 v; };
struct gd_BC_f32 { u8 pad[0xBC]; f32 v; };
struct gd_C0_f32 { u8 pad[0xC0]; f32 v; };
struct gd_C4_f32 { u8 pad[0xC4]; f32 v; };
struct gd_C8_f32 { u8 pad[0xC8]; f32 v; };
struct gd_CC_f32 { u8 pad[0xCC]; f32 v; };
struct gd_D0_f32 { u8 pad[0xD0]; f32 v; };
struct gd_D4_f32 { u8 pad[0xD4]; f32 v; };
struct gd_D8_f32 { u8 pad[0xD8]; f32 v; };
struct gd_DC_f32 { u8 pad[0xDC]; f32 v; };
struct gd_E0_f32 { u8 pad[0xE0]; f32 v; };
struct gd_E4_f32 { u8 pad[0xE4]; f32 v; };
struct gd_E8_f32 { u8 pad[0xE8]; f32 v; };
struct gd_EC_f32 { u8 pad[0xEC]; f32 v; };
struct gd_F0_f32 { u8 pad[0xF0]; f32 v; };
extern f64 D_8007B418;
extern f64 D_8007B420;
extern f64 D_8007B428;
extern f64 D_8007B430;
extern f32 D_80115410;
extern f32 D_8013ECA0;
f32 func_80056690(f32);

void f(s32 arg0) {
    f32 sp10;
    s32 pad0;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    s32 pad1;
    f32 sp28;
    f32 sp2C;
    f32 sp30;
    s32 pad2;
    f32 sp38;
    f32 sp3C;
    f32 sp40;
    s32 pad3;
    f32 sp48;
    f32 sp4C;
    f32 sp50;
    s32 pad4;
    f32 sp58;
    f32 sp5C;
    f32 sp60;
    s32 pad5;
    f32 sp68;
    f32 sp6C;
    f32 sp70;
    f64 sp78;

    sp18 = ((((struct gd_60_f32 *)arg0)->v + (((struct gd_6C_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_78_f32 *)arg0)->v * D_8013ECA0)) + (((struct gd_84_f32 *)arg0)->v * D_80115410);
    sp1C = ((((struct gd_64_f32 *)arg0)->v + (((struct gd_70_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_7C_f32 *)arg0)->v * D_8013ECA0)) + (((struct gd_88_f32 *)arg0)->v * D_80115410);
    sp20 = ((((struct gd_68_f32 *)arg0)->v + (((struct gd_74_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_80_f32 *)arg0)->v * D_8013ECA0)) + (((struct gd_8C_f32 *)arg0)->v * D_80115410);
    sp28 = ((((struct gd_60_f32 *)arg0)->v + (((struct gd_6C_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_78_f32 *)arg0)->v * (-D_8013ECA0))) + (((struct gd_84_f32 *)arg0)->v * D_80115410);
    sp2C = ((((struct gd_64_f32 *)arg0)->v + (((struct gd_70_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_7C_f32 *)arg0)->v * (-D_8013ECA0))) + (((struct gd_88_f32 *)arg0)->v * D_80115410);
    sp30 = ((((struct gd_68_f32 *)arg0)->v + (((struct gd_74_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_80_f32 *)arg0)->v * (-D_8013ECA0))) + (((struct gd_8C_f32 *)arg0)->v * D_80115410);
    sp38 = ((((struct gd_60_f32 *)arg0)->v + (((struct gd_6C_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_78_f32 *)arg0)->v * D_8013ECA0)) + (((struct gd_84_f32 *)arg0)->v * (-D_80115410));
    sp3C = ((((struct gd_64_f32 *)arg0)->v + (((struct gd_70_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_7C_f32 *)arg0)->v * D_8013ECA0)) + (((struct gd_88_f32 *)arg0)->v * (-D_80115410));
    sp40 = ((((struct gd_68_f32 *)arg0)->v + (((struct gd_74_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_80_f32 *)arg0)->v * D_8013ECA0)) + (((struct gd_8C_f32 *)arg0)->v * (-D_80115410));
    sp48 = ((((struct gd_60_f32 *)arg0)->v + (((struct gd_6C_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_78_f32 *)arg0)->v * (-D_8013ECA0))) + (((struct gd_84_f32 *)arg0)->v * (-D_80115410));
    sp4C = ((((struct gd_64_f32 *)arg0)->v + (((struct gd_70_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_7C_f32 *)arg0)->v * (-D_8013ECA0))) + (((struct gd_88_f32 *)arg0)->v * (-D_80115410));
    sp50 = ((((struct gd_68_f32 *)arg0)->v + (((struct gd_74_f32 *)arg0)->v * ((struct gd_F0_f32 *)arg0)->v)) + (((struct gd_80_f32 *)arg0)->v * (-D_8013ECA0))) + (((struct gd_8C_f32 *)arg0)->v * (-D_80115410));
    sp58 = sp18 - ((struct gd_60_f32 *)arg0)->v;
    sp5C = sp1C - ((struct gd_64_f32 *)arg0)->v;
    sp60 = sp20 - ((struct gd_68_f32 *)arg0)->v;
    sp68 = sp38 - ((struct gd_60_f32 *)arg0)->v;
    sp6C = sp3C - ((struct gd_64_f32 *)arg0)->v;
    sp70 = sp40 - ((struct gd_68_f32 *)arg0)->v;
    ((struct gd_9C_f32 *)arg0)->v = (sp6C * sp60) - (sp70 * sp5C);
    ((struct gd_A0_f32 *)arg0)->v = (sp70 * sp58) - (sp68 * sp60);
    ((struct gd_A4_f32 *)arg0)->v = (sp68 * sp5C) - (sp6C * sp58);
    sp10 = func_80056690((((((struct gd_9C_f32 *)arg0)->v * ((struct gd_9C_f32 *)arg0)->v) + (((struct gd_A0_f32 *)arg0)->v * ((struct gd_A0_f32 *)arg0)->v)) + (((struct gd_A4_f32 *)arg0)->v * ((struct gd_A4_f32 *)arg0)->v)));
    if (sp10 >= 1e-07f) {
        sp78 = D_8007B418 / ((f64)sp10);
        ((struct gd_9C_f32 *)arg0)->v = (f32)(((f64)((struct gd_9C_f32 *)arg0)->v) * sp78);
        ((struct gd_A0_f32 *)arg0)->v = (f32)(((f64)((struct gd_A0_f32 *)arg0)->v) * sp78);
        ((struct gd_A4_f32 *)arg0)->v = (f32)(((f64)((struct gd_A4_f32 *)arg0)->v) * sp78);
    } else {
    }
    ((struct gd_DC_f32 *)arg0)->v = ((sp18 * ((struct gd_9C_f32 *)arg0)->v) + (sp1C * ((struct gd_A0_f32 *)arg0)->v)) + (sp20 * ((struct gd_A4_f32 *)arg0)->v);
    sp58 = sp28 - ((struct gd_60_f32 *)arg0)->v;
    sp5C = sp2C - ((struct gd_64_f32 *)arg0)->v;
    sp60 = sp30 - ((struct gd_68_f32 *)arg0)->v;
    sp68 = sp18 - ((struct gd_60_f32 *)arg0)->v;
    sp6C = sp1C - ((struct gd_64_f32 *)arg0)->v;
    sp70 = sp20 - ((struct gd_68_f32 *)arg0)->v;
    ((struct gd_A8_f32 *)arg0)->v = (sp6C * sp60) - (sp70 * sp5C);
    ((struct gd_AC_f32 *)arg0)->v = (sp70 * sp58) - (sp68 * sp60);
    ((struct gd_B0_f32 *)arg0)->v = (sp68 * sp5C) - (sp6C * sp58);
    sp10 = func_80056690((((((struct gd_A8_f32 *)arg0)->v * ((struct gd_A8_f32 *)arg0)->v) + (((struct gd_AC_f32 *)arg0)->v * ((struct gd_AC_f32 *)arg0)->v)) + (((struct gd_B0_f32 *)arg0)->v * ((struct gd_B0_f32 *)arg0)->v)));
    if (sp10 >= 1e-07f) {
        sp78 = D_8007B420 / ((f64)sp10);
        ((struct gd_A8_f32 *)arg0)->v = (f32)(((f64)((struct gd_A8_f32 *)arg0)->v) * sp78);
        ((struct gd_AC_f32 *)arg0)->v = (f32)(((f64)((struct gd_AC_f32 *)arg0)->v) * sp78);
        ((struct gd_B0_f32 *)arg0)->v = (f32)(((f64)((struct gd_B0_f32 *)arg0)->v) * sp78);
    } else {
    }
    ((struct gd_E0_f32 *)arg0)->v = ((sp28 * ((struct gd_A8_f32 *)arg0)->v) + (sp2C * ((struct gd_AC_f32 *)arg0)->v)) + (sp30 * ((struct gd_B0_f32 *)arg0)->v);
    sp58 = sp48 - ((struct gd_60_f32 *)arg0)->v;
    sp5C = sp4C - ((struct gd_64_f32 *)arg0)->v;
    sp60 = sp50 - ((struct gd_68_f32 *)arg0)->v;
    sp68 = sp28 - ((struct gd_60_f32 *)arg0)->v;
    sp6C = sp2C - ((struct gd_64_f32 *)arg0)->v;
    sp70 = sp30 - ((struct gd_68_f32 *)arg0)->v;
    ((struct gd_90_f32 *)arg0)->v = (sp6C * sp60) - (sp70 * sp5C);
    ((struct gd_94_f32 *)arg0)->v = (sp70 * sp58) - (sp68 * sp60);
    ((struct gd_98_f32 *)arg0)->v = (sp68 * sp5C) - (sp6C * sp58);
    sp10 = func_80056690((((((struct gd_90_f32 *)arg0)->v * ((struct gd_90_f32 *)arg0)->v) + (((struct gd_94_f32 *)arg0)->v * ((struct gd_94_f32 *)arg0)->v)) + (((struct gd_98_f32 *)arg0)->v * ((struct gd_98_f32 *)arg0)->v)));
    if (sp10 >= 1e-07f) {
        sp78 = D_8007B428 / ((f64)sp10);
        ((struct gd_90_f32 *)arg0)->v = (f32)(((f64)((struct gd_90_f32 *)arg0)->v) * sp78);
        ((struct gd_94_f32 *)arg0)->v = (f32)(((f64)((struct gd_94_f32 *)arg0)->v) * sp78);
        ((struct gd_98_f32 *)arg0)->v = (f32)(((f64)((struct gd_98_f32 *)arg0)->v) * sp78);
    } else {
    }
    ((struct gd_D8_f32 *)arg0)->v = ((sp48 * ((struct gd_90_f32 *)arg0)->v) + (sp4C * ((struct gd_94_f32 *)arg0)->v)) + (sp50 * ((struct gd_98_f32 *)arg0)->v);
    sp58 = sp38 - ((struct gd_60_f32 *)arg0)->v;
    sp5C = sp3C - ((struct gd_64_f32 *)arg0)->v;
    sp60 = sp40 - ((struct gd_68_f32 *)arg0)->v;
    sp68 = sp48 - ((struct gd_60_f32 *)arg0)->v;
    sp6C = sp4C - ((struct gd_64_f32 *)arg0)->v;
    sp70 = sp50 - ((struct gd_68_f32 *)arg0)->v;
    ((struct gd_B4_f32 *)arg0)->v = (sp6C * sp60) - (sp70 * sp5C);
    ((struct gd_B8_f32 *)arg0)->v = (sp70 * sp58) - (sp68 * sp60);
    ((struct gd_BC_f32 *)arg0)->v = (sp68 * sp5C) - (sp6C * sp58);
    sp10 = func_80056690((((((struct gd_B4_f32 *)arg0)->v * ((struct gd_B4_f32 *)arg0)->v) + (((struct gd_B8_f32 *)arg0)->v * ((struct gd_B8_f32 *)arg0)->v)) + (((struct gd_BC_f32 *)arg0)->v * ((struct gd_BC_f32 *)arg0)->v)));
    if (sp10 >= 1e-07f) {
        sp78 = D_8007B430 / ((f64)sp10);
        ((struct gd_B4_f32 *)arg0)->v = (f32)(((f64)((struct gd_B4_f32 *)arg0)->v) * sp78);
        ((struct gd_B8_f32 *)arg0)->v = (f32)(((f64)((struct gd_B8_f32 *)arg0)->v) * sp78);
        ((struct gd_BC_f32 *)arg0)->v = (f32)(((f64)((struct gd_BC_f32 *)arg0)->v) * sp78);
    } else {
    }
    ((struct gd_E4_f32 *)arg0)->v = ((sp38 * ((struct gd_B4_f32 *)arg0)->v) + (sp3C * ((struct gd_B8_f32 *)arg0)->v)) + (sp40 * ((struct gd_BC_f32 *)arg0)->v);
    ((struct gd_CC_f32 *)arg0)->v = ((struct gd_6C_f32 *)arg0)->v;
    ((struct gd_D0_f32 *)arg0)->v = ((struct gd_70_f32 *)arg0)->v;
    ((struct gd_D4_f32 *)arg0)->v = ((struct gd_74_f32 *)arg0)->v;
    ((struct gd_EC_f32 *)arg0)->v = ((sp28 * ((struct gd_CC_f32 *)arg0)->v) + (sp2C * ((struct gd_D0_f32 *)arg0)->v)) + (sp30 * ((struct gd_D4_f32 *)arg0)->v);
    ((struct gd_C0_f32 *)arg0)->v = -((struct gd_6C_f32 *)arg0)->v;
    ((struct gd_C4_f32 *)arg0)->v = -((struct gd_70_f32 *)arg0)->v;
    ((struct gd_C8_f32 *)arg0)->v = -((struct gd_74_f32 *)arg0)->v;
    ((struct gd_E8_f32 *)arg0)->v = ((((struct gd_60_f32 *)arg0)->v * ((struct gd_C0_f32 *)arg0)->v) + (((struct gd_64_f32 *)arg0)->v * ((struct gd_C4_f32 *)arg0)->v)) + (((struct gd_68_f32 *)arg0)->v * ((struct gd_C8_f32 *)arg0)->v);
}
