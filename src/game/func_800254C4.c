#include "common.h"
#include "game_structs.h"



/* per-frame update: applies offset, calls placement, then registers a callback */
void func_800254C4(void *arg) {
    Actor *obj = arg;
    register f32 dx;
    register f32 dz;

    obj = arg;
    if (D_800AD03A == 0) {
        if (obj->u.model.animIndex.sub1d4.unk_2.h != 0) {
            func_80025390(&obj->u.model.animIndex.sub1d4);
        }
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            obj->unk_C.l1.flags |= 1;
        }
        return;
    }
    if (func_80012FD0((*(f32 (*)[3])&obj->pos)) == 0) {
        return;
    }
    if (obj->u.model.animIndex.sub1d4.unk_2.h != 0) {
        dx = (f32)(obj->u.model.animIndex.sub1d4.unk_0 * obj->u.model.animIndex.sub1d4.unk_2.b) * 0.001;
        dz = (f32)(obj->u.model.animIndex.sub1d4.unk_1 * obj->u.model.animIndex.sub1d4.unk_2.b) * 0.001;
        obj->pos.x += dx;
        obj->pos.z += dz;
    }
    ((void (*)(Sub40 *, f32 *, Vec3f))func_8001FAA8)(&obj->u.model.unk_E8.l0.unk_E8[D_800717E0], (*(f32 (*)[3])&obj->pos), obj->u.model.unk_1A8);
    if (obj->u.model.animIndex.sub1d4.unk_2.h != 0) {
        obj->pos.x -= dx;
        obj->pos.z -= dz;
    }
    addActorDL(func_800258C0, obj, obj->u.model.unk_1F0.l0.unk_1F0, 1, func_800236B4((*(f32 (*)[3])&obj->pos)));
}
