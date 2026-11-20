#include "common.h"
#include "game_structs.h"


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
extern Actor *D_80106C84;
extern u16 D_80107CB8;
extern u16 D_80107CBA;
extern Camera *D_80109D00;

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
    p->u.walker.turning = 0;
    if (p->u.walker.unk_1F0.l1.unk_1F2 < 0) {
        if (D_80109D00->unk_54 == 0 && p->u.walker.unk_1EC == 0) {
            p->u.walker.unk_1EC = 1;
            a = p->u.walker.offset.v.y - D_80106C84->pos.x;
            b = p->u.walker.speed - D_80106C84->pos.z;
            a = func_80069920(b, a);
            p->u.walker.goalDir.x = __cosf(a);
            p->u.walker.goalDir.z = __sinf(a);
        }
    } else if (p->u.walker.unk_1F0.l1.unk_1F3 == 0) {
        if (D_800BD858 == 3) {
            D_80109D00->unk_54 = 0x80;
            D_80109D00->unk_48 = (p->u.walker.offset.v.y - D_80109D00->unk_30) * 0.0078125;
            D_80109D00->unk_4C = (75.0 - D_80109D00->unk_34) * 0.0078125;
            D_80109D00->unk_50 = (p->u.walker.speed - D_80109D00->unk_38) * 0.0078125;
            D_80109D00->unk_10 = func_80032B28;
            p->u.walker.unk_1F0.l1.unk_1F2 = -1;
            D_8006FE58 &= ~0x82;
        }
        if (p->u.walker.unk_1F0.l2.unk_1F1 == 0 && p->u.walker.unk_1C8 == 0 && (D_80107CBA & 0xC00F)) {
            if (p->u.walker.unk_1F0.l2.unk_1F0 == 0) {
                if (D_8006FE5E != 0) {
                    func_800375A8(p, 0x21);
                    p->u.walker.unk_1F0.l2.unk_1F1 = 1;
                } else if (D_8011551C == NULL) {
                    D_8011551C = D_80074CAC;
                    osSendMesg(D_8013EEF8, 0, 1);
                }
            } else if (p->u.walker.unk_1F0.l2.unk_1F0 == 1) {
                if (D_8006FE5E < 9) {
                    func_800375A8(p, 0x1F);
                    p->u.walker.unk_1F0.l2.unk_1F1 = 2;
                }
            } else if (D_8006FE5E != 0) {
                (*(u8 (*))&D_8012FDE4) = 1;
                func_800375A8(p, 0x20);
                p->u.walker.unk_1ED = -1;
                p->u.walker.unk_1F0.l2.unk_1F1 = 3;
            } else if (D_8011551C == NULL) {
                D_8011551C = D_80074C7C;
                osSendMesg(D_8013EEF8, 0, 1);
            }
        }
        if (!(p->u.walker.unk_1C8 != 0x21 && p->u.walker.unk_1C8 != 0x1F && p->u.walker.unk_1C8 != 0x20 && (*(u8 (*))&D_8012FDE4) == 0)) {
            D_80107CBC = 0;
            D_80107CB8 &= ~0x300;
        }
        if (!(ABS(D_80107CBC) < 9 && !(D_80107CB8 & 0x300))) {
            if (HELD_RIGHT) {
                if (p->u.walker.unk_1F0.l2.unk_1F0 != 2) {
                    p->u.walker.unk_1F0.l2.unk_1F0++;
                    p->u.walker.unk_1F0.l1.unk_1F3 = 1;
                    p->u.walker.goalDir.x = 1.0f;
                    p->u.walker.goalDir.z = 0.0f;
                    p->u.walker.turning = 1;
                }
            } else if (p->u.walker.unk_1F0.l2.unk_1F0 != 0) {
                p->u.walker.unk_1F0.l2.unk_1F0--;
                p->u.walker.unk_1F0.l1.unk_1F3 = -1;
                p->u.walker.goalDir.x = -1.0f;
                p->u.walker.goalDir.z = 0.0f;
                p->u.walker.turning = 1;
            }
        }
    } else {
        p->u.walker.turning = 1;
        p->u.walker.unk_1A8.l0.scale.y = 5.0f;
        p->u.walker.unk_1A8.l0.unk_1B4 = 0;
        if (p->u.walker.unk_1F0.l1.unk_1F2 != 0) {
            if (ABS(D_80107CBC) < 8 && !(D_80107CB8 & 0x300)) {
                if (p->u.walker.unk_1F0.l2.unk_1F0 == 0) {
                    if (p->pos.x >= -100.0f) {
                        p->u.walker.unk_1F0.l2.unk_1F0 = 1;
                        p->u.walker.unk_1F0.l1.unk_1F3 = 1;
                        p->u.walker.goalDir.x = 1.0f;
                    }
                } else if (p->u.walker.unk_1F0.l2.unk_1F0 == 2 && p->pos.x <= 100.0f) {
                    p->u.walker.unk_1F0.l2.unk_1F0 = 1;
                    p->u.walker.unk_1F0.l1.unk_1F3 = -1;
                    p->u.walker.goalDir.x = -1.0f;
                }
            }
        }
        if (p->u.walker.unk_1F0.l1.unk_1F3 > 0) {
            if (HELD_LEFT) {
                p->u.walker.unk_1F0.l2.unk_1F0--;
                p->u.walker.unk_1F0.l1.unk_1F3 = -1;
                p->u.walker.goalDir.x = -1.0f;
            } else {
                p->pos.x += 5.0f;
                switch (p->u.walker.unk_1F0.l2.unk_1F0) {
                    case 1:
                        if (p->pos.x >= 0.0f) {
                            if (HELD_RIGHT) {
                                p->u.walker.unk_1F0.l2.unk_1F0 = 2;
                            } else {
                                p->u.walker.unk_1F0.l1.unk_1F2 = 0;
                                p->u.walker.unk_1F0.l1.unk_1F3 = 0;
                                p->u.walker.goalDir.x = 0.0f;
                                p->u.walker.goalDir.z = -1.0f;
                            }
                        }
                        break;
                    case 2:
                        if (p->pos.x >= 100.0f) {
                            p->u.walker.unk_1F0.l1.unk_1F2 = 1;
                        }
                        if (p->pos.x >= 400.0f) {
                            p->u.walker.unk_1F0.l1.unk_1F3 = 0;
                            p->u.walker.goalDir.x = 0.0f;
                            p->u.walker.goalDir.z = -1.0f;
                        }
                        break;
                }
            }
        } else if (HELD_RIGHT) {
            p->u.walker.unk_1F0.l2.unk_1F0++;
            p->u.walker.unk_1F0.l1.unk_1F3 = 1;
            p->u.walker.goalDir.x = 1.0f;
        } else {
            p->pos.x -= 5.0f;
            switch (p->u.walker.unk_1F0.l2.unk_1F0) {
                case 0:
                    if (p->pos.x <= -100.0f) {
                        p->u.walker.unk_1F0.l1.unk_1F2 = 1;
                    }
                    if (p->pos.x <= -400.0f) {
                        p->u.walker.unk_1F0.l1.unk_1F3 = 0;
                        p->u.walker.goalDir.x = 0.0f;
                        p->u.walker.goalDir.z = -1.0f;
                    }
                    break;
                case 1:
                    if (p->pos.x <= 0.0f) {
                        if (HELD_LEFT) {
                            p->u.walker.unk_1F0.l2.unk_1F0 = 0;
                        } else {
                            p->u.walker.unk_1F0.l1.unk_1F2 = 0;
                            p->u.walker.unk_1F0.l1.unk_1F3 = 0;
                            p->u.walker.goalDir.x = 0.0f;
                            p->u.walker.goalDir.z = -1.0f;
                        }
                    }
                    break;
            }
        }
    }
    a = func_80069920(p->fwd.x, p->fwd.z);
    b = func_80069920(p->u.walker.goalDir.x, p->u.walker.goalDir.z);
    b -= a;
    if (b > 3.14159) {
        b -= 6.2831853;
    } else if (b < -3.14159) {
        b += 6.2831853;
    }
    if (fabs(b) < 0.03) {
        p->fwd.x = p->u.walker.goalDir.x;
        p->fwd.y = p->u.walker.goalDir.y;
        p->fwd.z = p->u.walker.goalDir.z;
        if (p->u.walker.unk_1EC > 0) {
            p->u.walker.unk_1EC = -1;
        }
    } else {
        a = a + b * 0.2;
        p->fwd.x = __sinf(a);
        p->fwd.z = __cosf(a);
        p->u.walker.turning = 1;
        p->u.walker.unk_1A8.l0.scale.y = 5.0f;
        p->u.walker.unk_1A8.l0.unk_1B4 = 0;
    }
    p->side.x = p->fwd.y * p->up.z - p->fwd.z * p->up.y;
    p->side.y = p->fwd.z * p->up.x - p->fwd.x * p->up.z;
    p->side.z = p->fwd.x * p->up.y - p->fwd.y * p->up.x;
}
