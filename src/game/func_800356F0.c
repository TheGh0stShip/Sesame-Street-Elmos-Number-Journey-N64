#include "common.h"
#include "game_structs.h"


typedef struct Entry8 {
    s32 unk_0;
    s32 unk_4;
} Entry8;



typedef struct Target {
    u8 pad0[0x60];
    f32 unk_60;
    f32 unk_64;
    f32 unk_68;
} Target;
extern Entry8 D_80074280[];
extern Entry8 D_80074298[];
extern Entry8 D_800742D8[];
extern Entry8 D_8007462C[];
extern Entry8 D_800746BC[];
extern Entry8 D_8007474C[];
extern Actor *D_80106C84;
extern u16 D_80107CB8;
extern Entry8 *D_8013E7F0;
void func_8000EE10(Actor *);

/* Per-frame update for a collectible object: approach, pick-up animation and level completion. */
void func_800356F0(Actor *arg0) {
    register Actor *reg_s0;
    Actor *sp18;
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
            sp54 = func_80012FD0(&sp18->pos);
        } else {
            sp54 = 1;
        }
        if (sp54 != 0) {
            ((void (*)(Mtx40 *, Vec3f *, Vec3f))func_8001FAA8)(&sp18->u.pickup.unk_F8[D_800717E0], &sp18->pos, sp18->u.pickup.unk_58);
            addActorDL(D_8003670C, sp18, sp18->u.pickup.unk_1F0, 1, ((f32)sp20));
        }
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            sp18->unk_C.l1.flags = sp18->unk_C.l1.flags | 1;
        }
        return;
    }
    switch ((*(u8 (*))&D_8012F150)) {
    case 0:
        if (D_8012F154 != 19) {
            if (((D_80107CB8 & 0xC00F) & 0xFFFF) != 0) {
                if (sp18->u.pickup.anim == 0) {
                    if (D_80106C84->u.model.unk_1C8 != 34) {
                        if (D_80106C84->u.model.unk_1C8 != 35) {
                            if (D_8011551C == NULL) {
                                sp30 = D_80106C84->pos.x - sp18->pos.x;
                                sp38 = D_80106C84->pos.z - sp18->pos.z;
                                sp40 = (sp30 * sp30) + (sp38 * sp38);
                                if (sp40 < 1.6e+05f) {
                                    switch (D_8012F154) {
                                    case 9:
                                    case 10:
                                        D_80115404 = &D_8007462C[sp18->u.pickup.unk_1B8];
                                        break;
                                    case 7:
                                    case 8:
                                        D_80115404 = &D_800746BC[sp18->u.pickup.unk_1B8];
                                        break;
                                    case 5:
                                    case 6:
                                        D_80115404 = &D_8007474C[sp18->u.pickup.unk_1B8];
                                        break;
                                    }
                                    if (sp18->u.pickup.unk_1C0 != -1) {
                                        sp18->u.pickup.anim = 1;
                                        D_800A7F18 = 0;
                                        D_8013E7F0 = &D_80074298[func_800217C4(7)];
                                        D_8011551C = D_80074280;
                                        osSendMesg(D_8013EEF8, 0, 1);
                                    } else {
                                        sp18->u.pickup.anim = 2;
                                        D_800A7F18 = 0;
                                        D_8013E7F0 = &D_800742D8[func_800217C4(10)];
                                        D_8011551C = D_80074280;
                                        osSendMesg(D_8013EEF8, 0, 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else {
        }
        if (((f64)sp18->u.pickup.unk_1D0) == D_8007AEB0) {
            if (sp18->u.pickup.anim == 1) {
                func_800179D8(7);
            } else {
                if (sp18->u.pickup.anim == 2) {
                    func_800179D8(5);
                }
            }
        }
        if (((f64)sp18->u.pickup.unk_1D0) == D_8007AEB8) {
            switch (D_8012F154) {
            case 9:
            case 10:
                if (sp18->u.pickup.anim == 1) {
                    func_800179D8(22);
                } else {
                    func_800179D8(23);
                }
                break;
            case 7:
            case 8:
                if (sp18->u.pickup.anim == 1) {
                    func_800179D8(35);
                } else {
                    func_800179D8(36);
                }
                break;
            case 5:
                if (sp18->u.pickup.anim == 1) {
                    func_800179D8(25);
                } else {
                    func_800179D8(26);
                }
                break;
            case 6:
                if (sp18->u.pickup.anim == 1) {
                    func_800179D8(29);
                } else {
                    func_800179D8(30);
                }
                break;
            }
        }
        if ((sp18->u.pickup.unk_1D0 < 0.0f) && ((sp18->u.pickup.anim == 1) || (sp18->u.pickup.anim == 2))) {
            sp18->u.pickup.unk_1D0++;
        }
        if (((f64)sp18->u.pickup.unk_1D0) < D_8007AED8) {
            if (((f64)sp18->u.pickup.unk_1D0) >= 0.0) {
                if (sp18->u.pickup.anim == 1) {
                    if (((f64)sp18->u.pickup.unk_1D0) == D_8007AEE0) {
                        sp18->u.pickup.unk_54 = 0;
                        func_800179D8(6);
                    }
                    if (((f64)sp18->u.pickup.unk_1D0) < D_8007AEE8) {
                        reg_s0 = sp18;
                        reg_s0->fwd.x = __cosf(sp18->u.pickup.unk_1C4);
                        sp18->fwd.y = 0.0f;
                        reg_s0 = sp18;
                        reg_s0->fwd.z = __sinf(sp18->u.pickup.unk_1C4);
                        sp18->up.x = 0.0f;
                        sp18->up.y = 1.0f;
                        sp18->up.z = 0.0f;
                        sp18->pos.y = (f32)(((f64)sp18->pos.y) + (((f64)sp18->u.pickup.unk_1D0) * D_8007AEF0));
                        sp18->u.pickup.unk_1C4 = (f32)(((f64)sp18->u.pickup.unk_1C4) + D_8007AEF8);
                        if ((*(u8 (*))&D_8012F150) == 0) {
                            func_8000EE10(sp18);
                        }
                    } else {
                        if (((f64)sp18->u.pickup.unk_1D0) < D_8007AF00) {
                            sp18->pos.x = (f32)(((f64)sp18->pos.x) + (((f64)sp18->fwd.x) * D_8007AF08));
                            sp18->pos.z = (f32)(((f64)sp18->pos.z) + (((f64)sp18->fwd.z) * D_8007AF10));
                            sp18->pos.y = (f32)(((f64)sp18->pos.y) + D_8007AF18);
                            if ((*(u8 (*))&D_8012F150) == 0) {
                                func_8000EE10(sp18);
                            }
                        } else {
                            func_800179D8(1);
                            D_800ACFB0++;
                            D_800A93A4++;
                            D_800BD868[sp18->u.pickup.unk_1C0] = 1;
                            if ((D_8012F154 != 19) && ((D_800ACFB0 == D_8010E648) || (D_800A93A4 == D_800A9154))) {
                                func_800375A8(D_80106C84, 34);
                                if ((*(u8 (*))&D_8012F150) != 0) {
                                    D_800A91E4 = 0;
                                    D_80128443 = 0;
                                    (*(u8 (*))&D_8012F150) = 0;
                                    D_8006FE58 = D_8006FE58 | 112;
                                    D_8006FE58 = D_8006FE58 & -1551;
                                    if (D_8011551C != NULL) {
                                        D_8013C7C0 = (u32)D_80109DF6 >> 3;
                                        osStopTimer(D_8012F118);
                                    }
                                }
                            }
                            sp18->unk_C.l1.flags = sp18->unk_C.l1.flags | 1;
                            sp18->unk_C.l1.update = 0;
                            return;
                        }
                    }
                    sp18->u.pickup.unk_1D0++;
                } else {
                    if (sp18->u.pickup.anim == 2) {
                        if (((f64)sp18->u.pickup.unk_1D0) < D_8007AF20) {
                            sp44 = __cosf(sp18->u.pickup.unk_1C4);
                            sp48 = __sinf(sp18->u.pickup.unk_1C4);
                            sp4C = __cosf(sp18->u.pickup.unk_1C8);
                            sp50 = __sinf(sp18->u.pickup.unk_1C8);
                            sp18->fwd.x = sp44 * sp4C;
                            sp18->fwd.y = sp50;
                            sp18->fwd.z = sp48 * sp4C;
                            sp28 = sqrtf((((sp18->fwd.x * sp18->fwd.x) + (sp18->fwd.y * sp18->fwd.y)) + (sp18->fwd.z * sp18->fwd.z)));
                            if (sp28 >= 1e-07f) {
                                sp60 = D_8007AF28 / ((f64)sp28);
                                sp18->fwd.x = (f32)(((f64)sp18->fwd.x) * sp60);
                                sp18->fwd.y = (f32)(((f64)sp18->fwd.y) * sp60);
                                sp18->fwd.z = (f32)(((f64)sp18->fwd.z) * sp60);
                            } else {
                            }
                            sp18->up.x = (-sp44) * sp50;
                            sp18->up.y = sp4C;
                            sp18->up.z = (-sp48) * sp50;
                            if (((f64)sp18->u.pickup.unk_1C8) < (D_8007AF30 - ((f64)sp18->u.pickup.frame))) {
                                sp18->u.pickup.unk_1C8 = sp18->u.pickup.unk_1C8 + sp18->u.pickup.frame;
                                sp18->u.pickup.frame = (f32)(((f64)sp18->u.pickup.frame) + D_8007AF38);
                            }
                        } else {
                            if (((f64)sp18->u.pickup.unk_1D0) < D_8007AF40) {
                                sp18->pos.y = (f32)(((f64)sp18->pos.y) - D_8007AF48);
                            } else {
                                D_800A7A74++;
                                sp18->unk_C.l1.flags = sp18->unk_C.l1.flags | 1;
                                sp18->unk_C.l1.update = 0;
                                if (D_8012F154 != 19) {
                                    if (D_800A7A74 == D_800A7A58) {
                                        func_800375A8(D_80106C84, 34);
                                        if ((*(u8 (*))&D_8012F150) != 0) {
                                            D_800A91E4 = 0;
                                            D_80128443 = 0;
                                            (*(u8 (*))&D_8012F150) = 0;
                                            D_8006FE58 = D_8006FE58 | 112;
                                            D_8006FE58 = D_8006FE58 & -1551;
                                            if (D_8011551C != NULL) {
                                                D_8013C7C0 = (u32)D_80109DF6 >> 3;
                                                osStopTimer(D_8012F118);
                                            }
                                        }
                                    }
                                }
                                return;
                            }
                        }
                        sp18->u.pickup.unk_1D0++;
                    }
                }
                goto L878;
            }
        }
        if (D_8012F154 != 19) {
            sp30 = (*(Target *(*))&D_80112E90)->unk_60 - sp18->pos.x;
            sp38 = (*(Target *(*))&D_80112E90)->unk_68 - sp18->pos.z;
            sp18->u.pickup.unk_1C4 = (f32)func_80069920(((f64)sp38), ((f64)sp30));
            reg_s0 = sp18;
            reg_s0->fwd.x = __cosf(sp18->u.pickup.unk_1C4);
            reg_s0 = sp18;
            reg_s0->fwd.z = __sinf(sp18->u.pickup.unk_1C4);
            sp28 = sqrtf((((sp18->fwd.x * sp18->fwd.x) + (sp18->fwd.y * sp18->fwd.y)) + (sp18->fwd.z * sp18->fwd.z)));
            if (sp28 >= 1e-07f) {
                sp60 = D_8007AF50 / ((f64)sp28);
                sp18->fwd.x = (f32)(((f64)sp18->fwd.x) * sp60);
                sp18->fwd.y = (f32)(((f64)sp18->fwd.y) * sp60);
                sp18->fwd.z = (f32)(((f64)sp18->fwd.z) * sp60);
            } else {
L878: ;
            }
        }
        sp18->side.x = (sp18->fwd.y * sp18->up.z) - (sp18->fwd.z * sp18->up.y);
        sp18->side.y = (sp18->fwd.z * sp18->up.x) - (sp18->fwd.x * sp18->up.z);
        sp18->side.z = (sp18->fwd.x * sp18->up.y) - (sp18->fwd.y * sp18->up.x);
    }
}
