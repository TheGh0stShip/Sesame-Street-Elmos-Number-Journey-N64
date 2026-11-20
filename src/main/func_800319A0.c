typedef struct {
    u8 pad0[0x24];
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
    f32 unk_4C;
    f32 unk_50;
    u8 pad54[0x1AC - 0x54];
    f32 unk_1AC;
    u8 pad1B0[4];
    s32 unk_1B4;
    f32 unk_1B8;
    f32 unk_1BC;
    f32 unk_1C0;
    u8 pad1C4[4];
    s32 unk_1C8;
    u8 pad1CC[0x1D8 - 0x1CC];
    f32 unk_1D8;
    u8 pad1DC[4];
    f32 unk_1E0;
    u8 pad1E4[0x1EC - 0x1E4];
    s8 unk_1EC;
    s8 unk_1ED;
    u8 pad1EE[2];
    u8 unk_1F0;
    s8 unk_1F1;
    s8 unk_1F2;
    s8 unk_1F3;
    u8 pad1F4[0x204 - 0x1F4];
    s32 unk_204;
} Actor;

typedef struct {
    u8 pad0[0x10];
    void (*unk_10)();
    u8 pad14[0x30 - 0x14];
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    u8 pad3C[0x48 - 0x3C];
    f32 unk_48;
    f32 unk_4C;
    f32 unk_50;
    u8 unk_54;
} Camera;

extern void func_80032B28();
extern void func_800375A8(Actor *, s32);
extern f32 func_800566A0(f32);
extern f32 func_80057E70(f32);
extern void func_80058BA0(void *, s32, s32);
extern f64 func_80069920(f64, f64);
extern double fabs(double);
extern s32 D_8006FE58;
extern u8 D_8006FE5E;
extern u8 D_80074C7C[];
extern u8 D_80074CAC[];
extern u8 D_800BD858;
extern Actor *D_80106C84;
extern u16 D_80107CB8;
extern u16 D_80107CBA;
extern s8 D_80107CBC;
extern Camera *D_80109D00;
extern u8 *D_8011551C;
extern u8 D_8012FDE4;
extern u8 D_8013EEF8[];

#define ABS(x) ((x) >= 0.0f ? (x) : -(x))
#define HELD_RIGHT (!(D_80107CBC < 9 && !(D_80107CB8 & 0x100)))
#define HELD_LEFT (!(D_80107CBC >= -8 && !(D_80107CB8 & 0x200)))

/* steers the player actor through a three-lane selector from stick/button input */
void func_800319A0(void) {
    register Actor *p = D_80106C84;
    register f32 a;
    register f32 b;

    if (p == NULL) {
        return;
    }
    p->unk_204 = 0;
    if (p->unk_1F2 < 0) {
        if (D_80109D00->unk_54 == 0 && p->unk_1EC == 0) {
            p->unk_1EC = 1;
            a = p->unk_1D8 - D_80106C84->posX;
            b = p->unk_1E0 - D_80106C84->posZ;
            a = func_80069920(b, a);
            p->unk_1B8 = func_800566A0(a);
            p->unk_1C0 = func_80057E70(a);
        }
    } else if (p->unk_1F3 == 0) {
        if (D_800BD858 == 3) {
            D_80109D00->unk_54 = 0x80;
            D_80109D00->unk_48 = (p->unk_1D8 - D_80109D00->unk_30) * 0.0078125;
            D_80109D00->unk_4C = (75.0 - D_80109D00->unk_34) * 0.0078125;
            D_80109D00->unk_50 = (p->unk_1E0 - D_80109D00->unk_38) * 0.0078125;
            D_80109D00->unk_10 = func_80032B28;
            p->unk_1F2 = -1;
            D_8006FE58 &= ~0x82;
        }
        if (p->unk_1F1 == 0 && p->unk_1C8 == 0 && (D_80107CBA & 0xC00F)) {
            if (p->unk_1F0 == 0) {
                if (D_8006FE5E != 0) {
                    func_800375A8(p, 0x21);
                    p->unk_1F1 = 1;
                } else if (D_8011551C == NULL) {
                    D_8011551C = D_80074CAC;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
            } else if (p->unk_1F0 == 1) {
                if (D_8006FE5E < 9) {
                    func_800375A8(p, 0x1F);
                    p->unk_1F1 = 2;
                }
            } else if (D_8006FE5E != 0) {
                D_8012FDE4 = 1;
                func_800375A8(p, 0x20);
                p->unk_1ED = -1;
                p->unk_1F1 = 3;
            } else if (D_8011551C == NULL) {
                D_8011551C = D_80074C7C;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
        }
        if (!(p->unk_1C8 != 0x21 && p->unk_1C8 != 0x1F && p->unk_1C8 != 0x20 && D_8012FDE4 == 0)) {
            D_80107CBC = 0;
            D_80107CB8 &= ~0x300;
        }
        if (!(ABS(D_80107CBC) < 9 && !(D_80107CB8 & 0x300))) {
            if (HELD_RIGHT) {
                if (p->unk_1F0 != 2) {
                    p->unk_1F0++;
                    p->unk_1F3 = 1;
                    p->unk_1B8 = 1.0f;
                    p->unk_1C0 = 0.0f;
                    p->unk_204 = 1;
                }
            } else if (p->unk_1F0 != 0) {
                p->unk_1F0--;
                p->unk_1F3 = -1;
                p->unk_1B8 = -1.0f;
                p->unk_1C0 = 0.0f;
                p->unk_204 = 1;
            }
        }
    } else {
        p->unk_204 = 1;
        p->unk_1AC = 5.0f;
        p->unk_1B4 = 0;
        if (p->unk_1F2 != 0) {
            if (ABS(D_80107CBC) < 8 && !(D_80107CB8 & 0x300)) {
                if (p->unk_1F0 == 0) {
                    if (p->posX >= -100.0f) {
                        p->unk_1F0 = 1;
                        p->unk_1F3 = 1;
                        p->unk_1B8 = 1.0f;
                    }
                } else if (p->unk_1F0 == 2 && p->posX <= 100.0f) {
                    p->unk_1F0 = 1;
                    p->unk_1F3 = -1;
                    p->unk_1B8 = -1.0f;
                }
            }
        }
        if (p->unk_1F3 > 0) {
            if (HELD_LEFT) {
                p->unk_1F0--;
                p->unk_1F3 = -1;
                p->unk_1B8 = -1.0f;
            } else {
                p->posX += 5.0f;
                switch (p->unk_1F0) {
                    case 1:
                        if (p->posX >= 0.0f) {
                            if (HELD_RIGHT) {
                                p->unk_1F0 = 2;
                            } else {
                                p->unk_1F2 = 0;
                                p->unk_1F3 = 0;
                                p->unk_1B8 = 0.0f;
                                p->unk_1C0 = -1.0f;
                            }
                        }
                        break;
                    case 2:
                        if (p->posX >= 100.0f) {
                            p->unk_1F2 = 1;
                        }
                        if (p->posX >= 400.0f) {
                            p->unk_1F3 = 0;
                            p->unk_1B8 = 0.0f;
                            p->unk_1C0 = -1.0f;
                        }
                        break;
                }
            }
        } else if (HELD_RIGHT) {
            p->unk_1F0++;
            p->unk_1F3 = 1;
            p->unk_1B8 = 1.0f;
        } else {
            p->posX -= 5.0f;
            switch (p->unk_1F0) {
                case 0:
                    if (p->posX <= -100.0f) {
                        p->unk_1F2 = 1;
                    }
                    if (p->posX <= -400.0f) {
                        p->unk_1F3 = 0;
                        p->unk_1B8 = 0.0f;
                        p->unk_1C0 = -1.0f;
                    }
                    break;
                case 1:
                    if (p->posX <= 0.0f) {
                        if (HELD_LEFT) {
                            p->unk_1F0 = 0;
                        } else {
                            p->unk_1F2 = 0;
                            p->unk_1F3 = 0;
                            p->unk_1B8 = 0.0f;
                            p->unk_1C0 = -1.0f;
                        }
                    }
                    break;
            }
        }
    }
    a = func_80069920(p->unk_30, p->unk_38);
    b = func_80069920(p->unk_1B8, p->unk_1C0);
    b -= a;
    if (b > 3.14159) {
        b -= 6.2831853;
    } else if (b < -3.14159) {
        b += 6.2831853;
    }
    if (fabs(b) < 0.03) {
        p->unk_30 = p->unk_1B8;
        p->unk_34 = p->unk_1BC;
        p->unk_38 = p->unk_1C0;
        if (p->unk_1EC > 0) {
            p->unk_1EC = -1;
        }
    } else {
        a = a + b * 0.2;
        p->unk_30 = func_80057E70(a);
        p->unk_38 = func_800566A0(a);
        p->unk_204 = 1;
        p->unk_1AC = 5.0f;
        p->unk_1B4 = 0;
    }
    p->unk_48 = p->unk_34 * p->unk_44 - p->unk_38 * p->unk_40;
    p->unk_4C = p->unk_38 * p->unk_3C - p->unk_30 * p->unk_44;
    p->unk_50 = p->unk_30 * p->unk_40 - p->unk_34 * p->unk_3C;
}
