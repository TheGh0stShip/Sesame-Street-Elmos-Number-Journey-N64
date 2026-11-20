#include "common.h"

typedef struct { f32 x, y, z; } Vec3;
typedef struct {
    u8 pad0[0x48];
    f32 unk_48;
    Vec3 unk_4C;
    u8 pad58[4];
} Buf;
typedef struct {
    u8 pad0[8];
    u16 unk_8;
    u8 padA[2];
    s32 unk_C;
    u8 pad10[0x24 - 0x10];
    Vec3 unk_24;
    u8 pad30[0x60 - 0x30];
    Buf unk_60;
    u8 unk_BC[4];
} Model;
typedef struct {
    u8 pad0[0xC];
    s32 unk_C;
    u8 pad10[0x24 - 0x10];
    Vec3 unk_24;
    Vec3 unk_30;
    u8 pad3C[0x60 - 0x3C];
    Vec3 unk_60;
    Model *unk_6C;
    u8 pad70[4];
    s32 unk_74;
} Obj;
extern Obj *D_80106C84;

s32 func_800208D0(Buf *buf, void *arg1, Vec3 *arg2, Vec3 *arg3, f64 *arg4, s32 *arg5);

#define VSET(v, a, b, c) { (v)->x = (a); (v)->y = (b); (v)->z = (c); }
#define ABS(x) (((x) >= 0.0f) ? (x) : -(x))

/* refreshes an object's ground position/shadow scale from its model and notifies via callback */
void func_800181B0(Obj *arg0) {
    Obj *obj;
    f32 dist;
    Vec3 pos;
    f64 sp30[2];
    Vec3 *pp;
    s32 res;
    s32 flag;
    Model *m;
    Buf buf;
    void *spB0;

    if (D_800AD03A == 0) {
        return;
    }
    if (arg0 == NULL) {
        return;
    }
    obj = arg0;
    if (D_80109D4C >= 2) {
        if (D_80109D4C >= 8) {
            obj->unk_C |= 1;
        }
        return;
    }
    if ((obj->unk_74 & 1) || (obj->unk_6C->unk_C & 1)) {
        obj->unk_C |= 1;
        return;
    }
    switch (obj->unk_6C->unk_8) {
    default:
    case 5:
        m = obj->unk_6C;
        pp = &m->unk_24;
        buf = m->unk_60;
        spB0 = m->unk_BC;
        break;
    }
    pos.x = pp->x;
    pos.y = pp->y;
    pos.z = pp->z;
    if ((obj->unk_C & 0x4000) || ((obj->unk_6C->unk_C & 0x8000) &&
        (obj->unk_24.x != pos.x || obj->unk_24.z != pos.z || obj->unk_24.y != pos.y))) {
        obj->unk_C &= ~0x4000;
        pos.y = pos.y - 1200.0;
        flag = 0x80000000;
        res = func_800208D0(&buf, spB0, pp, &pos, sp30, &flag);
        if (res == 2) {
            VSET(&obj->unk_24, pp->x, pp->y, pp->z);
            obj->unk_24.y = buf.unk_48 + 5.0;
            VSET(&obj->unk_30, buf.unk_4C.x, buf.unk_4C.y, buf.unk_4C.z);
            dist = ABS(pp->y - obj->unk_24.y);
            dist = dist / 200.0;
        } else {
            dist = 10.0f;
        }
        if (dist == 0.0) {
            VSET(&obj->unk_60, 4.0f, 4.0f, 4.0f);
        } else if (dist > 4.0) {
            VSET(&obj->unk_60, 0.0f, 0.0f, 0.0f);
        } else {
            if (dist < 1.0) {
                dist = 1.0f;
            }
            VSET(&obj->unk_60, 4.0 / dist, 4.0 / dist, 4.0 / dist);
        }
        D_8013C720 = obj->unk_24.y;
        D_800A9148 = D_80106C84->unk_24.y - obj->unk_24.y;
        addActorDL(func_800186E8, obj, 5, 1, func_800236B4(&obj->unk_24));
    }
}
