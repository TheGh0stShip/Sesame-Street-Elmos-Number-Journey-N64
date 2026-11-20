
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Mtx40 {
    s32 m[4][4];
} Mtx40;

typedef struct Entry8 {
    s32 unk_0;
    s32 unk_4;
} Entry8;

typedef struct Obj {
    u8 pad0[0xC];
    s32 unk_0C;
    s32 unk_10;
    u8 pad14[0x24 - 0x14];
    Vec3f unk_24;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
    f32 unk_4C;
    f32 unk_50;
    s32 unk_54;
    Vec3f unk_58;
    u8 pad64[0xF8 - 0x64];
    Mtx40 unk_F8[3];
    u8 unk_1B8;
    s32 unk_1BC;
    s32 unk_1C0;
    f32 unk_1C4;
    f32 unk_1C8;
    f32 unk_1CC;
    f32 unk_1D0;
    u8 pad1D4[0x1F0 - 0x1D4];
    s32 unk_1F0;
} Obj;

typedef struct Player {
    u8 pad0[0x24];
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    u8 pad30[0x1C8 - 0x30];
    s32 unk_1C8;
} Player;

typedef struct Target {
    u8 pad0[0x60];
    f32 unk_60;
    f32 unk_64;
    f32 unk_68;
} Target;

extern u8 D_8003670C[];
extern s32 D_8006FE58;
extern s32 D_800717E0;
extern Entry8 D_80074280[];
extern Entry8 D_80074298[];
extern Entry8 D_800742D8[];
extern Entry8 D_8007462C[];
extern Entry8 D_800746BC[];
extern Entry8 D_8007474C[];
extern f64 D_8007AEB0;
extern f64 D_8007AEB8;
extern f64 D_8007AED8;
extern f64 D_8007AEE0;
extern f64 D_8007AEE8;
extern f64 D_8007AEF0;
extern f64 D_8007AEF8;
extern f64 D_8007AF00;
extern f64 D_8007AF08;
extern f64 D_8007AF10;
extern f64 D_8007AF18;
extern f64 D_8007AF20;
extern f64 D_8007AF28;
extern f64 D_8007AF30;
extern f64 D_8007AF38;
extern f64 D_8007AF40;
extern f64 D_8007AF48;
extern f64 D_8007AF50;
extern s32 D_800A7A58;
extern s32 D_800A7A74;
extern s16 D_800A7F18;
extern s32 D_800A9154;
extern s8 D_800A91E4;
extern s32 D_800A93A4;
extern s32 D_800ACFB0;
extern u8 D_800AD03A;
extern s8 D_800BD868[];
extern Player *D_80106C84;
extern u16 D_80107CB8;
extern s32 D_80109D4C;
extern u16 D_80109DF6;
extern s32 D_8010E648;
extern Target *D_80112E90;
extern Entry8 *D_80115404;
extern Entry8 *D_8011551C;
extern s8 D_80128443;
extern u8 D_8012F118[];
extern u8 D_8012F150;
extern s32 D_8012F154;
extern s16 D_8013C7C0;
extern Entry8 *D_8013E7F0;
extern u8 D_8013EEF8[];
void func_80001424(u8 *, Obj *, s32, s32, f32);
void func_8000EE10(Obj *);
s32 func_80012FD0(Vec3f *);
s32 func_800179D8();
void func_8001FAA8(Mtx40 *, Vec3f *, Vec3f);
s32 func_800217C4();
void func_800375A8(Player *, s32);
f32 func_80056690(f32);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
void func_80058BA0(u8 *, s32, s32);
void func_80058FC0(u8 *);
f64 func_80069920();

/* Per-frame update for a collectible object: approach, pick-up animation and level completion. */
void func_800356F0(Obj *arg0) {
    register Obj *reg_s0;
    Obj *sp18;
    f64 sp20;
    f32 sp28;
    s32 pad0;
    f32 sp30;
    s32 pad1;
    f32 sp38;
    s32 pad2;
    f32 sp40;
    f32 sp44;
    f32 sp48;
    f32 sp4C;
    f32 sp50;
    s32 sp54;
    s32 pad3[2];
    f64 sp60;

    sp20 = 0.0;
    sp18 = arg0;
    if (D_800AD03A != 0) {
        if (D_8012F154 != 19) {
            sp54 = func_80012FD0(&sp18->unk_24);
        } else {
            sp54 = 1;
        }
        if (sp54 != 0) {
            func_8001FAA8(&sp18->unk_F8[D_800717E0], &sp18->unk_24, sp18->unk_58);
            func_80001424(D_8003670C, sp18, sp18->unk_1F0, 1, ((f32)sp20));
        }
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            sp18->unk_0C = sp18->unk_0C | 1;
        }
        return;
    }
    switch (D_8012F150) {
    case 0:
        if (D_8012F154 != 19) {
            if (((D_80107CB8 & 0xC00F) & 0xFFFF) != 0) {
                if (sp18->unk_1BC == 0) {
                    if (D_80106C84->unk_1C8 != 34) {
                        if (D_80106C84->unk_1C8 != 35) {
                            if (D_8011551C == NULL) {
                                sp30 = D_80106C84->unk_24 - sp18->unk_24.x;
                                sp38 = D_80106C84->unk_2C - sp18->unk_24.z;
                                sp40 = (sp30 * sp30) + (sp38 * sp38);
                                if (sp40 < 1.6e+05f) {
                                    switch (D_8012F154) {
                                    case 9:
                                    case 10:
                                        D_80115404 = &D_8007462C[sp18->unk_1B8];
                                        break;
                                    case 7:
                                    case 8:
                                        D_80115404 = &D_800746BC[sp18->unk_1B8];
                                        break;
                                    case 5:
                                    case 6:
                                        D_80115404 = &D_8007474C[sp18->unk_1B8];
                                        break;
                                    }
                                    if (sp18->unk_1C0 != -1) {
                                        sp18->unk_1BC = 1;
                                        D_800A7F18 = 0;
                                        D_8013E7F0 = &D_80074298[func_800217C4(7)];
                                        D_8011551C = D_80074280;
                                        func_80058BA0(D_8013EEF8, 0, 1);
                                    } else {
                                        sp18->unk_1BC = 2;
                                        D_800A7F18 = 0;
                                        D_8013E7F0 = &D_800742D8[func_800217C4(10)];
                                        D_8011551C = D_80074280;
                                        func_80058BA0(D_8013EEF8, 0, 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else {
        }
        if (((f64)sp18->unk_1D0) == D_8007AEB0) {
            if (sp18->unk_1BC == 1) {
                func_800179D8(7);
            } else {
                if (sp18->unk_1BC == 2) {
                    func_800179D8(5);
                }
            }
        }
        if (((f64)sp18->unk_1D0) == D_8007AEB8) {
            switch (D_8012F154) {
            case 9:
            case 10:
                if (sp18->unk_1BC == 1) {
                    func_800179D8(22);
                } else {
                    func_800179D8(23);
                }
                break;
            case 7:
            case 8:
                if (sp18->unk_1BC == 1) {
                    func_800179D8(35);
                } else {
                    func_800179D8(36);
                }
                break;
            case 5:
                if (sp18->unk_1BC == 1) {
                    func_800179D8(25);
                } else {
                    func_800179D8(26);
                }
                break;
            case 6:
                if (sp18->unk_1BC == 1) {
                    func_800179D8(29);
                } else {
                    func_800179D8(30);
                }
                break;
            }
        }
        if ((sp18->unk_1D0 < 0.0f) && ((sp18->unk_1BC == 1) || (sp18->unk_1BC == 2))) {
            sp18->unk_1D0++;
        }
        if (((f64)sp18->unk_1D0) < D_8007AED8) {
            if (((f64)sp18->unk_1D0) >= 0.0) {
                if (sp18->unk_1BC == 1) {
                    if (((f64)sp18->unk_1D0) == D_8007AEE0) {
                        sp18->unk_54 = 0;
                        func_800179D8(6);
                    }
                    if (((f64)sp18->unk_1D0) < D_8007AEE8) {
                        reg_s0 = sp18;
                        reg_s0->unk_30 = func_800566A0(sp18->unk_1C4);
                        sp18->unk_34 = 0.0f;
                        reg_s0 = sp18;
                        reg_s0->unk_38 = func_80057E70(sp18->unk_1C4);
                        sp18->unk_3C = 0.0f;
                        sp18->unk_40 = 1.0f;
                        sp18->unk_44 = 0.0f;
                        sp18->unk_24.y = (f32)(((f64)sp18->unk_24.y) + (((f64)sp18->unk_1D0) * D_8007AEF0));
                        sp18->unk_1C4 = (f32)(((f64)sp18->unk_1C4) + D_8007AEF8);
                        if (D_8012F150 == 0) {
                            func_8000EE10(sp18);
                        }
                    } else {
                        if (((f64)sp18->unk_1D0) < D_8007AF00) {
                            sp18->unk_24.x = (f32)(((f64)sp18->unk_24.x) + (((f64)sp18->unk_30) * D_8007AF08));
                            sp18->unk_24.z = (f32)(((f64)sp18->unk_24.z) + (((f64)sp18->unk_38) * D_8007AF10));
                            sp18->unk_24.y = (f32)(((f64)sp18->unk_24.y) + D_8007AF18);
                            if (D_8012F150 == 0) {
                                func_8000EE10(sp18);
                            }
                        } else {
                            func_800179D8(1);
                            D_800ACFB0++;
                            D_800A93A4++;
                            D_800BD868[sp18->unk_1C0] = 1;
                            if ((D_8012F154 != 19) && ((D_800ACFB0 == D_8010E648) || (D_800A93A4 == D_800A9154))) {
                                func_800375A8(D_80106C84, 34);
                                if (D_8012F150 != 0) {
                                    D_800A91E4 = 0;
                                    D_80128443 = 0;
                                    D_8012F150 = 0;
                                    D_8006FE58 = D_8006FE58 | 112;
                                    D_8006FE58 = D_8006FE58 & -1551;
                                    if (D_8011551C != NULL) {
                                        D_8013C7C0 = (u32)D_80109DF6 >> 3;
                                        func_80058FC0(D_8012F118);
                                    }
                                }
                            }
                            sp18->unk_0C = sp18->unk_0C | 1;
                            sp18->unk_10 = 0;
                            return;
                        }
                    }
                    sp18->unk_1D0++;
                } else {
                    if (sp18->unk_1BC == 2) {
                        if (((f64)sp18->unk_1D0) < D_8007AF20) {
                            sp44 = func_800566A0(sp18->unk_1C4);
                            sp48 = func_80057E70(sp18->unk_1C4);
                            sp4C = func_800566A0(sp18->unk_1C8);
                            sp50 = func_80057E70(sp18->unk_1C8);
                            sp18->unk_30 = sp44 * sp4C;
                            sp18->unk_34 = sp50;
                            sp18->unk_38 = sp48 * sp4C;
                            sp28 = func_80056690((((sp18->unk_30 * sp18->unk_30) + (sp18->unk_34 * sp18->unk_34)) + (sp18->unk_38 * sp18->unk_38)));
                            if (sp28 >= 1e-07f) {
                                sp60 = D_8007AF28 / ((f64)sp28);
                                sp18->unk_30 = (f32)(((f64)sp18->unk_30) * sp60);
                                sp18->unk_34 = (f32)(((f64)sp18->unk_34) * sp60);
                                sp18->unk_38 = (f32)(((f64)sp18->unk_38) * sp60);
                            } else {
                            }
                            sp18->unk_3C = (-sp44) * sp50;
                            sp18->unk_40 = sp4C;
                            sp18->unk_44 = (-sp48) * sp50;
                            if (((f64)sp18->unk_1C8) < (D_8007AF30 - ((f64)sp18->unk_1CC))) {
                                sp18->unk_1C8 = sp18->unk_1C8 + sp18->unk_1CC;
                                sp18->unk_1CC = (f32)(((f64)sp18->unk_1CC) + D_8007AF38);
                            }
                        } else {
                            if (((f64)sp18->unk_1D0) < D_8007AF40) {
                                sp18->unk_24.y = (f32)(((f64)sp18->unk_24.y) - D_8007AF48);
                            } else {
                                D_800A7A74++;
                                sp18->unk_0C = sp18->unk_0C | 1;
                                sp18->unk_10 = 0;
                                if (D_8012F154 != 19) {
                                    if (D_800A7A74 == D_800A7A58) {
                                        func_800375A8(D_80106C84, 34);
                                        if (D_8012F150 != 0) {
                                            D_800A91E4 = 0;
                                            D_80128443 = 0;
                                            D_8012F150 = 0;
                                            D_8006FE58 = D_8006FE58 | 112;
                                            D_8006FE58 = D_8006FE58 & -1551;
                                            if (D_8011551C != NULL) {
                                                D_8013C7C0 = (u32)D_80109DF6 >> 3;
                                                func_80058FC0(D_8012F118);
                                            }
                                        }
                                    }
                                }
                                return;
                            }
                        }
                        sp18->unk_1D0++;
                    }
                }
                goto L878;
            }
        }
        if (D_8012F154 != 19) {
            sp30 = D_80112E90->unk_60 - sp18->unk_24.x;
            sp38 = D_80112E90->unk_68 - sp18->unk_24.z;
            sp18->unk_1C4 = (f32)func_80069920(((f64)sp38), ((f64)sp30));
            reg_s0 = sp18;
            reg_s0->unk_30 = func_800566A0(sp18->unk_1C4);
            reg_s0 = sp18;
            reg_s0->unk_38 = func_80057E70(sp18->unk_1C4);
            sp28 = func_80056690((((sp18->unk_30 * sp18->unk_30) + (sp18->unk_34 * sp18->unk_34)) + (sp18->unk_38 * sp18->unk_38)));
            if (sp28 >= 1e-07f) {
                sp60 = D_8007AF50 / ((f64)sp28);
                sp18->unk_30 = (f32)(((f64)sp18->unk_30) * sp60);
                sp18->unk_34 = (f32)(((f64)sp18->unk_34) * sp60);
                sp18->unk_38 = (f32)(((f64)sp18->unk_38) * sp60);
            } else {
L878: ;
            }
        }
        sp18->unk_48 = (sp18->unk_34 * sp18->unk_44) - (sp18->unk_38 * sp18->unk_40);
        sp18->unk_4C = (sp18->unk_38 * sp18->unk_3C) - (sp18->unk_30 * sp18->unk_44);
        sp18->unk_50 = (sp18->unk_30 * sp18->unk_40) - (sp18->unk_34 * sp18->unk_3C);
    }
}
