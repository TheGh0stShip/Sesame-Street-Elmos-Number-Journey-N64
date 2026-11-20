struct gd_2_u16 { u8 pad[0x2]; u16 v; };
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
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
extern s32 D_8002336C;
extern s32 D_80023408;
extern s32 D_8004BD4C;
extern s32 D_8004C060;
extern s32 D_8004C0FC;
extern s32 D_8004CA34;
extern s32 D_8004CC4C;
extern s32 D_8004CEE4;
extern s32 D_8004CFA8;
extern s32 D_8004D2FC;
extern s32 D_8004DA70;
extern s32 D_8004DDB0;
extern s32 D_80075030;
extern s32 D_80076800;
extern s32 D_8007A6A0;
extern s32 D_8007A6A4;
extern s32 D_800A7F1C;
extern s32 D_800AD010;
extern s32 D_800AF040;
extern s32 D_80112E90;
extern s32 D_80115408;
extern s32 D_80115524;
extern s32 D_8012F154;
extern s32 D_8012FE48;
extern s32 D_80133FB8;
s32 func_80033058();
s32 func_80033A6C();
s32 func_80041A80();
s32 func_80042080();
s32 func_80049970(s32, s32, f32, f32);
s32 func_8004B208(s32, s32, s32, s32, s32, f32);
s32 func_8004B47C(s32, s32, s32, s32, s32, f32);
s32 func_8004BB10(s32, s32, f32, s32, f32, f32, f32);
s32 func_8004BC54();
s32 func_8004C0A4();
s32 func_8004CAA0(s32, s32, s32, s32, s32, f32, f32, f32, s32);
s32 func_8004CBF4();
s32 func_8004CF28();
s32 func_8004D340();
s32 func_8004DA14();
s32 func_80059540();
f64 func_80069920();
s32 func_80069C70();

void f(s32 arg0) {
    s32 sp28;
    s32 pad0;
    s32 pad1[2];
    f32 sp38;
    f32 sp3C;
    f32 sp40;
    s32 pad2;
    f32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 pad3;
    f32 sp58;
    f32 sp5C;
    f32 sp60;
    s32 pad4;
    f32 sp68;
    f32 sp6C;
    f32 sp70;
    s32 pad5;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    s32 pad6;
    s32 sp88;
    s32 sp8C;
    s32 sp90;
    s32 sp94;
    s32 pad7;
    s32 sp9C;
    s32 pad8[4];
    f32 spB0;

    sp28 = arg0;
    if ((*(s32 *)sp28) == 1) {
        goto L13;
    }
    func_80059540(((s32)&D_8007A6A0), ((s32)&D_8007A6A4), 62);
L13: ;
    if (D_8012F154 == 11) {
        func_80033058(arg0);
        return;
    }
    if (D_8012F154 == 19) {
        func_80033A6C(arg0);
        return;
    }
    D_80076800 = func_80042080(180);
    func_80069C70(D_80076800, 0, 180);
    ((struct gd_8_s16 *)D_80076800)->v = ((struct gd_2_u16 *)sp28)->v;
    ((struct gd_A_s16 *)D_80076800)->v = ((struct gd_6_u16 *)sp28)->v;
    ((struct gd_C_s32 *)D_80076800)->v = ((struct gd_8_s32 *)sp28)->v;
    ((struct gd_14_s32 *)D_80076800)->v = (s32)&D_80023408;
    ((struct gd_18_s32 *)D_80076800)->v = 0;
    ((struct gd_1C_s32 *)D_80076800)->v = 0;
    ((struct gd_20_s32 *)D_80076800)->v = 0;
    ((struct gd_10_s32 *)D_80076800)->v = (s32)&D_8002336C;
    func_80041A80(D_80076800, D_80115408, 3, 0);
    D_80112E90 = 0;
    D_80133FB8 = 0;
    D_8012FE48 = 0;
    D_80115524 = 0;
    D_800AD010 = 0;
    D_800AF040 = 0;
    D_800A7F1C = 0;
    sp38 = ((struct gd_C_f32 *)sp28)->v;
    sp3C = ((struct gd_10_f32 *)sp28)->v;
    sp40 = ((struct gd_14_f32 *)sp28)->v;
    sp48 = (f32)func_80069920(((f64)((struct gd_20_f32 *)sp28)->v), ((f64)((struct gd_18_f32 *)sp28)->v));
    sp4C = 0;
    sp50 = 0;
    sp78 = ((struct gd_18_f32 *)sp28)->v;
    sp7C = ((struct gd_1C_f32 *)sp28)->v;
    sp80 = ((struct gd_20_f32 *)sp28)->v;
    switch (D_8012F154) {
    case 10:
        spB0 = 9e+02f;
        break;
    case 5:
        spB0 = 1.08e+03f;
        break;
    case 6:
        spB0 = 9e+02f;
        break;
    case 8:
        spB0 = 5.5e+02f;
        break;
    case 13:
        spB0 = 8.3e+02f;
        break;
    case 14:
        spB0 = 8.2e+02f;
        break;
    case 15:
    case 16:
        spB0 = 7.2e+02f;
        break;
    case 1:
    case 2:
        spB0 = 6.5e+02f;
        break;
    case 17:
        spB0 = 7e+02f;
        break;
    case 18:
        spB0 = 6.5e+02f;
        break;
    default:
        spB0 = 6e+02f;
        break;
    }
    D_80112E90 = func_80049970(((s32)&sp38), ((s32)&sp48), spB0, ((f32)(*(s32 *)((D_8012F154 * 4) + ((s32)&D_80075030)))));
    switch (D_8012F154) {
    case 10:
        D_80133FB8 = func_8004BB10(D_80112E90, 0x3F000000, spB0, 0x42F00000, 2e+01f, 24.0f, 0.4f);
        break;
    case 5:
        D_80133FB8 = func_8004BB10(D_80112E90, 0x3F4CCCCD, spB0, 0x43020000, 45.0f, 18.0f, -0.3f);
        break;
    case 6:
        D_80133FB8 = func_8004BB10(D_80112E90, 0x40000000, spB0, 0x433E0000, 65.0f, 18.0f, -0.3f);
        break;
    case 8:
        D_80133FB8 = func_8004BB10(D_80112E90, 0x3F000000, spB0, 0x42C80000, 4e+01f, 24.0f, -0.4f);
        break;
    case 1:
        D_80133FB8 = func_8004BB10(D_80112E90, 0x3F000000, spB0, 0x42C80000, 1.2e+02f, 24.0f, 0.0f);
        break;
    case 9:
        D_80133FB8 = func_8004BB10(D_80112E90, 0x3F000000, spB0, 0x428C0000, 5e+01f, 24.0f, 0.25f);
        break;
    default:
        D_80133FB8 = func_8004BB10(D_80112E90, 0x3F000000, spB0, 0x428C0000, 5e+01f, 24.0f, 0.4f);
        break;
    }
    sp88 = func_8004B208(D_80112E90, D_80133FB8, ((s32)&D_8004BD4C), ((s32)&D_8004C060), 10, 1.0f);
    func_8004BC54(D_80133FB8, ((s32)&sp38), ((s32)&sp78));
    D_8012FE48 = func_8004C0A4(D_80112E90);
    sp8C = func_8004B208(D_80112E90, D_8012FE48, ((s32)&D_8004C0FC), ((s32)&D_8004CA34), 20, 1.0f);
    D_80115524 = func_8004CBF4(D_80112E90);
    sp90 = func_8004B208(D_80112E90, D_80115524, ((s32)&D_8004CC4C), ((s32)&D_8004CEE4), 30, 1.0f);
    switch (D_8012F154) {
    case 6:
        break;
    case 5:
    case 10:
        D_800AD010 = func_8004CF28(D_80112E90, 0x447A0000, 0x3E4CCCCD, 0x447A0000);
        sp94 = func_8004B208(D_80112E90, D_800AD010, ((s32)&D_8004CFA8), ((s32)&D_8004D2FC), 50, 1.0f);
        break;
    case 1:
        D_800AD010 = func_8004CF28(D_80112E90, 0x42200000, 0x3D23D70A, 0x420C0000);
        sp94 = func_8004B208(D_80112E90, D_800AD010, ((s32)&D_8004CFA8), ((s32)&D_8004D2FC), 50, 1.0f);
        break;
    default:
        D_800AD010 = func_8004CF28(D_80112E90, 0x42200000, 0x3F000000, 0x420C0000);
        sp94 = func_8004B208(D_80112E90, D_800AD010, ((s32)&D_8004CFA8), ((s32)&D_8004D2FC), 50, 1.0f);
        break;
    }
    D_800AF040 = func_8004D340(D_80112E90, arg0, ((s32)&sp38), 170);
    if (D_8012F154 != 1) {
        if (D_8012F154 != 2) {
            if (D_8012F154 != 11) {
                D_800A7F1C = func_8004DA14(D_80112E90);
                sp9C = func_8004B208(D_80112E90, D_800A7F1C, ((s32)&D_8004DA70), ((s32)&D_8004DDB0), 70, 1.0f);
            }
        }
    }
    if ((D_8012F154 == 1) || (D_8012F154 == 2)) {
        sp58 = 1.6e+03f;
        sp60 = -1.2e+03f;
        sp5C = 5e+02f;
        func_8004CAA0(D_8012FE48, 2, 1, ((s32)&sp58), ((s32)&sp58), 2e+04f, 2e+04f, 2e+04f, 0);
        func_8004B47C(D_80112E90, ((s32)&sp58), 0, 1, 0, 1e+03f);
        sp58 = 2e+01f;
        sp60 = -555.0f;
        sp5C = 1e+02f;
        sp68 = 2e+01f;
        sp70 = -4.8e+02f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 2, ((s32)&sp58), ((s32)&sp68), 1.5e+02f, 2e+02f, 5e+01f, 0);
        sp68 = 2e+01f;
        sp70 = -5.3e+02f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 3, ((s32)&sp58), ((s32)&sp68), 1.5e+02f, 2e+02f, 25.0f, 0);
        sp68 = 2e+01f;
        sp70 = -5.8e+02f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 4, ((s32)&sp58), ((s32)&sp68), 1.5e+02f, 2e+02f, 25.0f, 0);
        sp68 = 2e+01f;
        sp70 = -635.0f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 5, ((s32)&sp58), ((s32)&sp68), 1.5e+02f, 2e+02f, 55.0f, 0);
        sp68 = -1.6e+02f;
        sp70 = -555.0f;
        sp6C = 2e+02f;
        sp58 = 4e+02f;
        sp60 = -555.0f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 2, 0, 6e+02f);
        sp58 = 4e+02f;
        sp60 = -655.0f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 3, 0, 6e+02f);
        sp58 = 4e+02f;
        sp60 = -455.0f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 4, 0, 6e+02f);
        sp58 = 4e+02f;
        sp60 = -575.0f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 5, 0, 6e+02f);
        sp58 = 1643.0f;
        sp60 = 8.6e+02f;
        sp5C = 1e+02f;
        sp68 = 1493.0f;
        sp70 = 8.6e+02f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 6, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 1e+02f, 2.5e+02f, 0);
        sp68 = 1618.0f;
        sp70 = 8.6e+02f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 7, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 25.0f, 2.5e+02f, 0);
        sp68 = 1668.0f;
        sp70 = 8.6e+02f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 8, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 25.0f, 2.5e+02f, 0);
        sp68 = 1793.0f;
        sp70 = 8.6e+02f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 9, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 1e+02f, 2.5e+02f, 0);
        sp68 = 1643.0f;
        sp70 = 8.6e+02f;
        sp6C = 2e+02f;
        sp58 = 1703.0f;
        sp60 = 4e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 9, 0, 6e+02f);
        sp58 = 1593.0f;
        sp60 = 4e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 8, 0, 6e+02f);
        sp58 = 1693.0f;
        sp60 = 4e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 7, 0, 6e+02f);
        sp58 = 1603.0f;
        sp60 = 4e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 6, 0, 6e+02f);
        sp58 = 2.6e+02f;
        sp60 = 842.0f;
        sp5C = 1e+02f;
        sp68 = 2.6e+02f;
        sp70 = 842.0f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 10, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 2e+02f, 2e+02f, 0);
        sp68 = 2.6e+02f;
        sp70 = 862.0f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 11, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 2e+02f, 2e+01f, 0);
        sp68 = 2.6e+02f;
        sp70 = 822.0f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 12, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 2e+02f, 2e+01f, 0);
        sp68 = 2.7e+02f;
        sp70 = 842.0f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 13, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 1e+01f, 2e+02f, 0);
        sp68 = 2.4e+02f;
        sp70 = 842.0f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 14, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 2e+01f, 2e+02f, 0);
        sp68 = 2e+02f;
        sp70 = 842.0f;
        sp6C = 1e+02f;
        func_8004CAA0(D_8012FE48, 2, 15, ((s32)&sp58), ((s32)&sp68), 2.5e+02f, 2e+01f, 2e+02f, 0);
        sp68 = 2.6e+02f;
        sp70 = 842.0f;
        sp6C = 2e+02f;
        sp58 = 6.5e+02f;
        sp60 = 6.1e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 10, 0, 7.2e+02f);
        sp58 = 6.5e+02f;
        sp60 = 5.9e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 11, 0, 7.2e+02f);
        sp58 = 625.0f;
        sp60 = 5e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 12, 0, 7.2e+02f);
        sp58 = 4.5e+02f;
        sp60 = 5e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 13, 0, 7.2e+02f);
        sp58 = 375.0f;
        sp60 = 5e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 14, 0, 7.2e+02f);
        sp58 = 325.0f;
        sp60 = 4.5e+02f;
        sp5C = 2.4e+02f;
        func_8004B47C(D_80112E90, ((s32)&sp58), ((s32)&sp68), 15, 0, 7.2e+02f);
    }
}
