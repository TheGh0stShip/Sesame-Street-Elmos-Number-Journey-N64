#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;

/* builds the object's view matrix, or places it 1100 units from the player at height 225 */
void func_80032924(Actor *arg0) {
    Mtx44 m0;
    Mtx44 m1;
    Mtx44 m2;
    register Actor *o = arg0;
    register f32 dx;
    register f32 dz;
    register f32 s;

    if (D_800AD03A != 0) {
        ((void (*)(Mtx44 *, u8 *, s32, f32, f32, f32, f32))guPerspectiveF)(&m0, (*(u8 (*)[])&D_800A7F00), D_80108FB4, 320.0 / D_800B1092, 50.0f, 10000.0f, 1.0f);
        guLookAtF(&m1, o->pos.x, o->pos.y, o->pos.z, o->fwd.x, o->fwd.y, o->fwd.z, o->up.x, o->up.y, o->up.z);
        guMtxCatF(&m1, &m0, &m2);
        guMtxF2L(&m2, D_8013EF28);
        addActorDL(func_80032894, o, 1, 0, 0);
    } else {
        dx = D_80106C84->pos.x - o->fwd.x;
        dz = D_80106C84->pos.z - o->fwd.z;
        s = 1100.0 / sqrtf(dx * dx + dz * dz);
        dx = dx * s;
        dz = dz * s;
        o->pos.x = D_80106C84->pos.x + dx;
        o->pos.y = 225.0f;
        o->pos.z = D_80106C84->pos.z + dz;
    }
}
