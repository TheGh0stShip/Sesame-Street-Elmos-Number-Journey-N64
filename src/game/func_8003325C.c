#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;
/* distance check against player object; triggers on close approach */
void func_8003325C(Actor *arg0) {
    register Actor *o = arg0;
    register f32 dx;
    register f32 dz;
    s32 pad;
    if (D_800AD03A != 0) return;
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) o->unk_C.l1.flags |= 1;
        return;
    }
    dx = o->pos.x - D_80106C84->pos.x;
    dz = o->pos.z - D_80106C84->pos.z;
    dx = dx * dx;
    dz = dz * dz;
    dz = dz + dx;
    if (dz < 300000.0) {
        func_800375A8(D_80106C84, 0x22);
        o->unk_C.l1.flags |= 1;
        o->unk_C.l1.update = 0;
    }
}
