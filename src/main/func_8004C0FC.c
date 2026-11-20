struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_30_s32 { u8 pad[0x30]; s32 v; };
extern f64 D_8007B490;
extern f64 D_8007B498;
extern f64 D_8007B4A0;
extern s32 D_800A7F0C;
s32 func_8004B5C8();
f32 func_80056690(f32);
s32 func_80060E7C();
f64 func_800694D0();
f64 func_80069920();

void f(s32 arg0, s32 arg1, s32 arg2) {
    register s32 reg_s0;
    register s32 reg_s1;
    register s32 reg_s2;
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    s32 pad1;
    f32 sp30;
    f32 sp34;
    f64 sp38;

    reg_s0 = arg0;
    func_80060E7C(arg2, arg1, 40);
    reg_s2 = ((struct gd_4_s32 *)reg_s0)->v;
    while (!(reg_s2 == 0)) {
        switch (*(s32 *)reg_s2) {
        case 0:
            sp20 = (*(f32 *)arg2) - ((struct gd_8_f32 *)reg_s2)->v;
            sp24 = ((struct gd_4_f32 *)arg2)->v - ((struct gd_C_f32 *)reg_s2)->v;
            sp28 = ((struct gd_8_f32 *)arg2)->v - ((struct gd_10_f32 *)reg_s2)->v;
            sp30 = ((sp20 * sp20) + (sp24 * sp24)) + (sp28 * sp28);
            if (sp30 <= ((struct gd_14_f32 *)reg_s2)->v) {
                reg_s1 = func_8004B5C8((*(s32 *)reg_s0), ((struct gd_4_s32 *)reg_s2)->v);
                if (reg_s1 != 0) {
                    D_800A7F0C = ((struct gd_4_s32 *)reg_s2)->v;
                    (*(f32 *)arg2) = *(f32 *)arg1;
                    ((struct gd_4_f32 *)arg2)->v = ((struct gd_4_f32 *)arg1)->v;
                    ((struct gd_8_f32 *)arg2)->v = ((struct gd_8_f32 *)arg1)->v;
                    if (((struct gd_8_s32 *)reg_s1)->v != 0) {
                        (*(f32 *)arg2) = ((*(f32 *)arg1) + ((struct gd_1C_f32 *)reg_s1)->v) / 2.0f;
                        ((struct gd_4_f32 *)arg2)->v = (((struct gd_4_f32 *)arg2)->v + ((struct gd_20_f32 *)reg_s1)->v) / 2.0f;
                        ((struct gd_8_f32 *)arg2)->v = (((struct gd_8_f32 *)arg2)->v + ((struct gd_24_f32 *)reg_s1)->v) / 2.0f;
                    }
                    sp10 = (*(f32 *)arg2) - ((struct gd_10_f32 *)reg_s1)->v;
                    sp14 = ((struct gd_4_f32 *)arg2)->v - ((struct gd_14_f32 *)reg_s1)->v;
                    sp18 = ((struct gd_8_f32 *)arg2)->v - ((struct gd_18_f32 *)reg_s1)->v;
                    sp34 = func_80056690((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
                    if (sp34 >= 1e-07f) {
                        sp38 = D_8007B490 / ((f64)sp34);
                        sp10 = (f32)(((f64)sp10) * sp38);
                        sp14 = (f32)(((f64)sp14) * sp38);
                        sp18 = (f32)(((f64)sp18) * sp38);
                    } else {
                    }
                    ((struct gd_C_f32 *)arg2)->v = (f32)func_80069920(((f64)sp18), ((f64)sp10));
                    ((struct gd_10_f32 *)arg2)->v = (f32)func_800694D0(((f64)(-sp14)));
                    ((struct gd_18_f32 *)arg2)->v = ((struct gd_C_f32 *)reg_s1)->v;
                    ((struct gd_24_s32 *)arg2)->v = ((struct gd_24_s32 *)arg2)->v | 16;
                }
            }
            break;
        case 1:
            sp20 = (*(f32 *)arg2) - ((struct gd_8_f32 *)reg_s2)->v;
            sp28 = ((struct gd_8_f32 *)arg2)->v - ((struct gd_10_f32 *)reg_s2)->v;
            sp30 = (sp20 * sp20) + (sp28 * sp28);
            if (sp30 <= ((struct gd_14_f32 *)reg_s2)->v) {
                reg_s1 = func_8004B5C8((*(s32 *)reg_s0), ((struct gd_4_s32 *)reg_s2)->v);
                if (reg_s1 != 0) {
                    D_800A7F0C = ((struct gd_4_s32 *)reg_s2)->v;
                    (*(f32 *)arg2) = *(f32 *)arg1;
                    ((struct gd_4_f32 *)arg2)->v = ((struct gd_4_f32 *)arg1)->v;
                    ((struct gd_8_f32 *)arg2)->v = ((struct gd_8_f32 *)arg1)->v;
                    if (((struct gd_8_s32 *)reg_s1)->v != 0) {
                        (*(f32 *)arg2) = ((*(f32 *)arg2) + ((struct gd_1C_f32 *)reg_s1)->v) / 2.0f;
                        ((struct gd_4_f32 *)arg2)->v = (((struct gd_4_f32 *)arg2)->v + ((struct gd_20_f32 *)reg_s1)->v) / 2.0f;
                        ((struct gd_8_f32 *)arg2)->v = (((struct gd_8_f32 *)arg2)->v + ((struct gd_24_f32 *)reg_s1)->v) / 2.0f;
                    }
                    sp10 = (*(f32 *)arg2) - ((struct gd_10_f32 *)reg_s1)->v;
                    sp14 = ((struct gd_4_f32 *)arg2)->v - ((struct gd_14_f32 *)reg_s1)->v;
                    sp18 = ((struct gd_8_f32 *)arg2)->v - ((struct gd_18_f32 *)reg_s1)->v;
                    sp34 = func_80056690((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
                    if (sp34 >= 1e-07f) {
                        sp38 = D_8007B498 / ((f64)sp34);
                        sp10 = (f32)(((f64)sp10) * sp38);
                        sp14 = (f32)(((f64)sp14) * sp38);
                        sp18 = (f32)(((f64)sp18) * sp38);
                    } else {
                    }
                    ((struct gd_C_f32 *)arg2)->v = (f32)func_80069920(((f64)sp18), ((f64)sp10));
                    ((struct gd_10_f32 *)arg2)->v = (f32)func_800694D0(((f64)(-sp14)));
                    ((struct gd_18_f32 *)arg2)->v = ((struct gd_C_f32 *)reg_s1)->v;
                    ((struct gd_24_s32 *)arg2)->v = ((struct gd_24_s32 *)arg2)->v | 16;
                }
            }
            break;
        case 2:
            sp20 = (*(f32 *)arg2) - ((struct gd_8_f32 *)reg_s2)->v;
            sp28 = ((struct gd_8_f32 *)arg2)->v - ((struct gd_10_f32 *)reg_s2)->v;
            sp30 = (sp20 * sp20) + (sp28 * sp28);
            if (sp30 <= ((struct gd_14_f32 *)reg_s2)->v) {
                if ((*(f32 *)arg2) > ((struct gd_18_f32 *)reg_s2)->v) {
                    if ((*(f32 *)arg2) < ((struct gd_1C_f32 *)reg_s2)->v) {
                        if (((struct gd_8_f32 *)arg2)->v > ((struct gd_20_f32 *)reg_s2)->v) {
                            if (((struct gd_8_f32 *)arg2)->v < ((struct gd_24_f32 *)reg_s2)->v) {
                                reg_s1 = func_8004B5C8((*(s32 *)reg_s0), ((struct gd_4_s32 *)reg_s2)->v);
                                if (reg_s1 != 0) {
                                    D_800A7F0C = ((struct gd_4_s32 *)reg_s2)->v;
                                    (*(f32 *)arg2) = *(f32 *)arg1;
                                    ((struct gd_4_f32 *)arg2)->v = ((struct gd_4_f32 *)arg1)->v;
                                    ((struct gd_8_f32 *)arg2)->v = ((struct gd_8_f32 *)arg1)->v;
                                    if (((struct gd_8_s32 *)reg_s1)->v != 0) {
                                        (*(f32 *)arg2) = ((*(f32 *)arg2) + ((struct gd_1C_f32 *)reg_s1)->v) / 2.0f;
                                        ((struct gd_4_f32 *)arg2)->v = (((struct gd_4_f32 *)arg2)->v + ((struct gd_20_f32 *)reg_s1)->v) / 2.0f;
                                        ((struct gd_8_f32 *)arg2)->v = (((struct gd_8_f32 *)arg2)->v + ((struct gd_24_f32 *)reg_s1)->v) / 2.0f;
                                    }
                                    sp10 = (*(f32 *)arg2) - ((struct gd_10_f32 *)reg_s1)->v;
                                    sp14 = ((struct gd_4_f32 *)arg2)->v - ((struct gd_14_f32 *)reg_s1)->v;
                                    sp18 = ((struct gd_8_f32 *)arg2)->v - ((struct gd_18_f32 *)reg_s1)->v;
                                    sp34 = func_80056690((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
                                    if (sp34 >= 1e-07f) {
                                        sp38 = D_8007B4A0 / ((f64)sp34);
                                        sp10 = (f32)(((f64)sp10) * sp38);
                                        sp14 = (f32)(((f64)sp14) * sp38);
                                        sp18 = (f32)(((f64)sp18) * sp38);
                                    } else {
                                    }
                                    ((struct gd_C_f32 *)arg2)->v = (f32)func_80069920(((f64)sp18), ((f64)sp10));
                                    ((struct gd_10_f32 *)arg2)->v = (f32)func_800694D0(((f64)(-sp14)));
                                    ((struct gd_18_f32 *)arg2)->v = ((struct gd_C_f32 *)reg_s1)->v;
                                    ((struct gd_24_s32 *)arg2)->v = ((struct gd_24_s32 *)arg2)->v | 16;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        reg_s2 = ((struct gd_30_s32 *)reg_s2)->v;
    }
}
