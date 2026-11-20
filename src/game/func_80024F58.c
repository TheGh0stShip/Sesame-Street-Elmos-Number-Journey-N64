#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;

void func_80024F58(Actor *arg0) {
    Actor *o;
    u8 pad1C[0x64];
    s32 unk88;
    s32 pad8C[1];
    s32 pad90;
    s32 flag;
    f64 d;
    s32 padA0;
    register f32 dx;
    register f32 dz;

    o = arg0;
    unk88 = 0;
    flag = 0;
    if (D_800AD03A == 0) {
        if (o->unk_8.l1.unk_A == 0x1001) {
            dx = o->pos.x - D_80106C84->pos.x;
            dz = o->pos.z - D_80106C84->pos.z;
            dx = dx * dx;
            dz = dz * dz;
            dz = dz + dx;
            D_8012FE44 = dz;
            if (dz < 30000000.0) {
                func_80010D54(o->pos.x, (s32)o->pos.y + 10, o->pos.z);
            }
            if (o->u.prop.unk_1D4.ub == 0) {
                if (dz < 12000000.0) {
                    o->u.prop.unk_1D4.ub = 1;
                    o->u.prop.unk_1D8 = func_8004E5A0(0x22);
                }
            } else if (dz > 13000000.0) {
                o->u.prop.unk_1D4.ub = 0;
                func_8004E884(o->u.prop.unk_1D8, 0);
            } else {
                dx = sqrtf(dz);
                dx = 3400.0 - dx;
                if (dx < 0.0f) {
                    dx = 0.0f;
                }
                dx = dx * 0.075294118;
                ((void (*)(s32, s32))func_8004E990)(o->u.prop.unk_1D8, dx);
            }
        }
    } else {
        d = 0;
        o = arg0;
        if (D_80109D4C >= 5) {
            if (D_80109D4C >= 8) {
                o->unk_C.l1.flags = o->unk_C.l1.flags | 1;
            }
            return;
        }
        if (o->unk_8.l1.unk_A == 0x5004 || o->unk_8.l1.unk_A == 0x5006 || o->unk_8.l1.unk_A == 0x5505 || o->unk_8.l1.unk_A == 0x4001 ||
            o->unk_8.l1.unk_A == 0x4002 || o->unk_8.l1.unk_A == 0x2505 || o->unk_8.l1.unk_A == 0x2005 || o->unk_8.l1.unk_A == 0x2006 ||
            o->unk_8.l1.unk_A == 0x2009 || o->unk_8.l1.unk_A == 0x4609 || o->unk_8.l1.unk_A == 0x1502) {
            flag = 1;
        }
        if (flag == 0 && func_80012FD0(&o->pos.x) == 0) {
            return;
        }
        if (o->unk_8.l1.unk_A != 0x1015 && o->unk_8.l1.unk_A != 0x7016 && o->unk_8.l1.unk_A != 0x7017 && o->u.prop.unk_1E8 != 0) {
            ((void (*)(Elem40 *, f32 *, Vec3f))func_8001FAA8)(&o->u.prop.unk_E8[D_800717E0], &o->pos.x, o->u.prop.scale);
            addActorDL(func_800258C0, o, o->u.prop.unk_1F0, 1, d);
        }
    }
}
