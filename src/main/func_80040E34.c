typedef struct { s32 m[16]; } Mtx;
typedef struct { f32 x, y, z; } Vec3;
typedef struct { u16 unk_00; u8 unk_02[6]; s32 unk_08; s32 unk_0C; u8 unk_10[0xC]; s32 unk_1C; } Model;
typedef struct { u8 unk_00[4]; u8 unk_04; } Anim;
typedef struct {
    u8 unk_00[0xC]; s32 unk_0C; s32 unk_10; Mtx *unk_14; s32 unk_18; u8 unk_1C[4]; u16 unk_20;
} Inst;
typedef struct Actor {
    u8 unk_000[0xA]; u16 type; s32 flags; u8 unk_010[0x14];
    Vec3 pos; f32 fwd[3]; f32 up[3]; f32 side[3];
    u8 unk_054[0xE8 - 0x54]; Mtx mtx[2];
    u8 unk_168[0x1A8 - 0x168]; Vec3 scale; u8 unk_1B4[8];
    Anim *anim; u8 unk_1C0[0xC]; s32 frame; u8 unk_1D0[4]; s32 timer; Anim *nextAnim;
    u8 sfxFrame; u8 sfx; u8 unk_1DE[2]; u8 hidden; u8 unk_1E1[7]; Model *model;
    u8 unk_1EC[4]; u8 unk_1F0; s8 state; u8 unk_1F2[2]; Inst inst;
} Actor;

void func_8000092C(Model *, Inst *, Anim *, s32, Vec3, f32 *, Mtx *, Actor *);
void func_80001424(void *, Actor *, s32, s32, f32);
void func_80003D68(void);
void func_80018E08(void);
void func_80018F60(void);
void func_8001FAA8(Mtx *, Vec3 *, Vec3);
s32 func_800217C4(s32);
f64 func_800236B4(Vec3 *);
void func_800375A8(Actor *, s32);
void func_8004E5A0(s32);
f32 func_80056690(f32);
void func_80058BA0(void *, s32, s32);
extern u8 D_8004189C[];
extern u8 D_8006FE5E;
extern u8 D_8006FE60;
extern s32 D_800717E0;
extern u8 D_80074270[];
extern s32 D_80074CDC[];
extern void *D_80074CE8[];
extern void *D_80074CF0[];
extern u8 D_800A7F70;
extern u8 D_800AD03A;
extern u8 D_800BD858;
extern s32 D_80106C7C;
extern Actor *D_80106C84;
extern s32 D_80115404;
extern void *D_8011551C;
extern u8 D_8012F150;
extern s8 D_8012FDE4;
extern u8 D_8013EEF8[];
extern Anim D_8016DA50;
extern Anim D_80170E34;
extern Anim D_80177388;
extern Anim D_801792B4;
extern Anim D_801811D0;
extern Anim D_80184FA8;
extern Anim D_801A2BD8;
extern Anim D_801A2E58;

/* Per-frame update of a scripted character: animation, orientation and per-type sequence logic. */
void func_80040E34(Actor *arg0) {
    s32 unused1;
    f32 len;
    Inst *p;
    f64 inv;
    Vec3 unused2;
    register Actor *a;

    len = 1.0f;
    a = arg0;
    a->flags = a->flags | 0x8000;
    if (D_800AD03A != 0) {
        if (a->hidden == 0) {
            func_8001FAA8(&a->mtx[D_800717E0], &a->pos, a->scale);
            if (a->anim != NULL) {
                func_8000092C(a->model, &a->inst, a->anim, a->frame, a->pos, a->fwd, &a->mtx[D_800717E0], a);
            } else {
                p = &a->inst;
                p->unk_14 = &a->mtx[D_800717E0];
                p->unk_0C = a->model->unk_0C;
                p->unk_10 = a->model->unk_08;
                p->unk_18 = a->model->unk_1C;
                p->unk_20 = a->model->unk_00;
            }
            func_80001424(D_8004189C, a, 4, 1, func_800236B4(&a->pos));
        }
        return;
    }
    if (D_8012F150 != 0) {
        return;
    }
    if (a->anim != NULL && !(D_80106C7C & 1)) {
        a->frame++;
        if (a->frame >= (&a->anim->unk_04)[0]) {
            a->flags = a->flags | 8;
            if (!(a->flags & 0x80)) {
                a->frame = 0;
            }
        }
    }
    len = func_80056690(a->fwd[0] * a->fwd[0] + a->fwd[1] * a->fwd[1] + a->fwd[2] * a->fwd[2]);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        a->fwd[0] = a->fwd[0] * inv;
        a->fwd[1] = a->fwd[1] * inv;
        a->fwd[2] = a->fwd[2] * inv;
    } else {
    }
    if (len > 1e-7f) {
        a->side[0] = a->fwd[1] * a->up[2] - a->fwd[2] * a->up[1];
        a->side[1] = a->fwd[2] * a->up[0] - a->fwd[0] * a->up[2];
        a->side[2] = a->fwd[0] * a->up[1] - a->fwd[1] * a->up[0];
    }
    switch (a->type) {
    case 0x5001:
        if (a->timer != 0) {
            if (a->timer > 0) {
                if (--a->timer == 0) {
                    a->timer = -1;
                    a->anim = &D_80184FA8;
                    a->flags = a->flags & ~8;
                    a->frame = 0;
                }
            } else if (a->flags & 8) {
                a->timer = 0;
                a->anim = &D_801811D0;
                a->frame = 0;
                D_80106C84->state = 0;
            }
        } else if (D_80106C84->state == 2) {
            D_80106C84->state = 5;
            a->timer = 80;
        }
        break;
    case 0x5003:
        if (a->timer != 0) {
            if (a->timer > 0) {
                if (--a->timer == 0) {
                    a->timer = -1;
                    a->anim = &D_801792B4;
                    a->flags = a->flags & ~8;
                    a->frame = 0;
                }
            } else if (a->flags & 8) {
                a->timer = 0;
                a->anim = &D_80177388;
                a->frame = 0;
            } else if (a->frame == 8) {
                func_8004E5A0(0x2F);
            }
        } else if (D_80106C84->state == 1) {
            D_80106C84->state = 4;
            a->timer = 20;
        }
        break;
    case 0x5005:
        if (a->timer != 0) {
            if (a->timer > 0) {
                if (--a->timer == 0) {
                    a->timer = -1;
                    a->anim = &D_80170E34;
                    a->flags = a->flags & ~8;
                    a->frame = 0;
                }
            } else if (a->flags & 8) {
                a->timer = 0;
                a->anim = &D_8016DA50;
                a->frame = 0;
                D_80106C84->state = 0;
                if (D_8006FE5E != D_800A7F70) {
                    func_8004E5A0(0x2A);
                    if (++D_8006FE60 == 3) {
                        if (++D_800BD858 < 3) {
                            func_80018F60();
                            if (D_8011551C == NULL) {
                                func_80018E08();
                            }
                            D_8006FE60 = 0;
                            func_80003D68();
                        } else {
                            func_800375A8(D_80106C84, 0x22);
                        }
                    } else if (D_8006FE5E < D_800A7F70) {
                        if (D_8011551C == NULL) {
                            D_8011551C = D_80074CE8[func_800217C4(2)];
                            func_80058BA0(D_8013EEF8, 0, 1);
                        }
                    } else if (D_8011551C == NULL) {
                        D_8011551C = D_80074CF0[func_800217C4(3)];
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                } else {
                    func_8004E5A0(0x29);
                    if (++D_800BD858 < 3) {
                        func_80018F60();
                        if (D_8011551C == NULL) {
                            func_80018E08();
                        }
                        D_8006FE60 = 0;
                        func_80003D68();
                    }
                }
                D_8012FDE4 = 0;
            } else if (a->frame == 15) {
                func_8004E5A0(0x32);
            }
        } else if (D_80106C84->state == 3) {
            a->timer = 20;
        }
        break;
    case 0x6000:
        if (a->timer != 0) {
            if (a->timer > 0) {
                if (--a->timer == 0) {
                    a->timer = -1;
                    a->hidden = 0;
                    a->anim = a->nextAnim;
                    a->flags = a->flags & ~8;
                    a->frame = 0;
                }
            } else if (a->flags & 8) {
                if (a->nextAnim == &D_801A2BD8) {
                    D_80106C84->state = 0;
                }
                a->hidden = 1;
                a->timer = 0;
                a->anim = NULL;
                if (a->nextAnim == &D_801A2E58) {
                    func_8004E5A0(0x2E);
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074270;
                        D_80115404 = D_80074CDC[func_800217C4(3)];
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                }
            } else if (a->frame == a->sfxFrame) {
                func_8004E5A0(a->sfx);
            }
        } else if (D_80106C84->state == 4) {
            D_80106C84->state = 7;
            a->nextAnim = &D_801A2BD8;
            a->timer = 40;
            a->sfxFrame = 5;
            a->sfx = 0x31;
        } else if (D_80106C84->state == 5) {
            D_80106C84->state = 8;
            a->nextAnim = &D_801A2E58;
            a->timer = 1;
            a->sfxFrame = 0x35;
            a->sfx = 0x2C;
        }
        break;
    }
}
