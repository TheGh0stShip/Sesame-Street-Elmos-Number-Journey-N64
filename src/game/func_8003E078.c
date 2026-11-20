#include "common.h"

/* enemy/NPC update: faces the player, then runs its state machine (idle, approach, chase, follow path) */
typedef struct { f32 x, y, z; } Vec3;
typedef struct { u8 pad0[4]; u8 len[4]; } Anim;
typedef struct Obj {
    u8 pad0[0xA]; u16 unk_0A; s32 flags;
    u8 pad10[0x14];
    Vec3 pos;
    Vec3 dir;
    Vec3 up;
    Vec3 side;
    u8 pad54[0xE8 - 0x54];
    f32 unk_E8[3][16];
    Vec3 unk_1A8;
    u8 pad1B4[8];
    Anim *anim;
    u8 pad1C0[0xC];
    s32 frame;
    u8 pad1D0[8];
    u8 state; u8 sub; u8 unk_1DA; u8 unk_1DB; u8 unk_1DC; u8 unk_1DD; u8 pad1DE[2];
    f32 speed;
    u8 pad1E4[4];
    s32 unk_1E8;
    u8 pad1EC[4];
    s32 unk_1F0;
    s32 unk_1F4;
} Obj;
typedef struct { u8 pad[0x60]; f32 x; f32 y; f32 z; } Player;
extern Anim **D_800774A4[];
extern Obj *D_80106C84;

#define VDOT(a, b) ((a)->x * (b)->x + (a)->y * (b)->y + (a)->z * (b)->z)

void func_8003E078(Obj *arg0) {
    register Obj *t;
    register Obj *a;
    Obj *obj;
    f32 unk30[16];
    f32 dist;
    f32 angle;
    f32 dxx;
    f32 dzz;
    f32 limit;
    Vec3 d;
    Vec3 unk98;
    Vec3 unkA8;
    Vec3 toP;
    Anim **tbl;
    Vec3 tgt;
    Vec3 pos;
    Vec3 unkF0;
    Vec3 v;
    f64 zero;
    f64 mag;

    zero = 0;
    obj = arg0;
    if (D_800AD03A != 0) {
        ((void (*)(f32 *, Vec3 *, Vec3))func_8001FAA8)(obj->unk_E8[D_800717E0], &obj->pos, obj->unk_1A8);
        ((void (*)(s32, s32 *, Anim *, s32, Vec3, Vec3 *, f32 *, Obj *))func_8000092C)(obj->unk_1E8, &obj->unk_1F4, obj->anim, obj->frame, obj->pos, &obj->dir,
                      obj->unk_E8[D_800717E0], obj);
        addActorDL(func_8003EE1C, obj, obj->unk_1F0, 1, zero);
    } else if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            obj->flags |= 1;
        }
    } else if ((*(u8 (*))&D_8012F150) != 0) {
    } else {
        if (obj->state != 4) {
            toP.x = (*(Player *(*))&D_80112E90)->x - obj->pos.x;
            toP.z = (*(Player *(*))&D_80112E90)->z - obj->pos.z;
            angle = func_80069920(toP.z, toP.x);
            a = obj;
            a->dir.x = __cosf(angle);
            a = obj;
            a->dir.z = __sinf(angle);
            mag = sqrtf(VDOT(&obj->dir, &obj->dir));
            if (mag != 0.0) {
                (&obj->dir)->x = (&obj->dir)->x * (1.0 / mag);
                (&obj->dir)->y = (&obj->dir)->y * (1.0 / mag);
                (&obj->dir)->z = (&obj->dir)->z * (1.0 / mag);
            }
            obj->side.x = obj->dir.y * obj->up.z - obj->dir.z * obj->up.y;
            obj->side.y = obj->dir.z * obj->up.x - obj->dir.x * obj->up.z;
            obj->side.z = obj->dir.x * obj->up.y - obj->dir.y * obj->up.x;
        }
        switch (obj->state) {
        case 1:
            obj->flags |= 0x8000;
            obj->flags &= ~0x80;
            if (obj->anim != NULL) {
                if (!(D_80106C7C & 1)) {
                    obj->frame++;
                    if (obj->frame > *obj->anim->len) {
                        obj->flags |= 8;
                        if (!(obj->flags & 0x80)) {
                            obj->frame = 0;
                        }
                    }
                }
            } else {
            }
            break;
        case 2:
            switch (obj->sub) {
            case 1:
                t = D_80106C84;
                if (t == NULL) {
                    return;
                }
                d.x = obj->pos.x - t->pos.x;
                d.z = obj->pos.z - t->pos.z;
                dist = d.x * d.x + d.z * d.z;
                if (dist < 160000.0) {
                    obj->sub = 2;
                    obj->unk_1DA = 1;
                    obj->frame = 0;
                    obj->flags |= 8;
                    tbl = D_800774A4[obj->unk_1DB];
                    obj->anim = tbl[obj->unk_1DA];
                    switch (obj->unk_0A) {
                    case 0x7101:
                        func_800179D8(0x28);
                        break;
                    case 0x8102:
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 2:
                break;
            }
            obj->flags |= 0x8000;
            obj->flags &= ~0x80;
            if (obj->anim != NULL && !(D_80106C7C & 1)) {
                obj->frame++;
                if (obj->frame >= *obj->anim->len) {
                    obj->flags |= 8;
                    obj->frame = 0;
                    obj->sub = 1;
                    obj->unk_1DA = 0;
                    tbl = D_800774A4[obj->unk_1DB];
                    obj->anim = tbl[obj->unk_1DA];
                }
            }
            break;
        case 3:
            switch (obj->sub) {
            case 1:
                t = D_80106C84;
                if (t == NULL) {
                    return;
                }
                d.x = obj->pos.x - t->pos.x;
                d.z = obj->pos.z - t->pos.z;
                dist = d.x * d.x + d.z * d.z;
                if (obj->unk_0A == 0x4601) {
                    limit = 810000.0f;
                } else {
                    limit = 160000.0f;
                }
                if (dist < limit) {
                    obj->sub = 2;
                    switch (obj->unk_0A) {
                    case 0x6102:
                        break;
                    case 0x6104:
                        func_800179D8(0x14);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 2:
                obj->flags |= 0x8000;
                obj->flags |= 0x80;
                if (obj->anim != NULL && !(D_80106C7C & 1)) {
                    obj->frame++;
                    if (obj->frame >= *obj->anim->len) {
                        obj->sub = 3;
                    }
                }
                break;
            case 3:
                break;
            }
            break;
        case 4:
            obj->flags |= 0x8000;
            obj->flags &= ~0x80;
            if (obj->anim != NULL && !(D_80106C7C & 1)) {
                obj->frame++;
                if (obj->frame > *obj->anim->len) {
                    obj->flags |= 8;
                    if (!(obj->flags & 0x80)) {
                        obj->frame = 0;
                    }
                }
            }
            goto L1;
        L1:
            goto L2;
        L2:
            v.x = tgt.x;
            v.y = tgt.y;
            v.z = tgt.z;
            pos.x = (&obj->pos)->x;
            pos.y = (&obj->pos)->y;
            pos.z = (&obj->pos)->z;
            v.x = v.x - pos.x;
            v.y = v.y - pos.y;
            v.z = v.z - pos.z;
            mag = sqrtf(VDOT(&v, &v));
            if (mag != 0.0) {
                v.x = v.x * (1.0 / mag);
                v.y = v.y * (1.0 / mag);
                v.z = v.z * (1.0 / mag);
            }
            (&obj->dir)->x = v.x;
            (&obj->dir)->y = v.y;
            (&obj->dir)->z = v.z;
            obj->side.x = obj->dir.y * obj->up.z - obj->dir.z * obj->up.y;
            obj->side.y = obj->dir.z * obj->up.x - obj->dir.x * obj->up.z;
            obj->side.z = obj->dir.x * obj->up.y - obj->dir.y * obj->up.x;
            obj->pos.x += v.x * obj->speed;
            obj->pos.z += v.z * obj->speed;
            dxx = obj->pos.x - tgt.x;
            dxx = dxx * dxx;
            dzz = obj->pos.z - tgt.z;
            dzz = dzz * dzz;
            if (dxx + dzz <= 400.0f) {
                obj->unk_1DC++;
                obj->unk_1DC %= obj->unk_1DD;
            }
            break;
        default:
            break;
        }
    }
}
