#include "common.h"
#include "game_structs.h"

/* faces the object toward a fixed point and computes a cross product, or switches its update handler */
void func_8003A1B4(Actor *o) {
    register f32 dx;
    register f32 dz;
    register Actor *p;
    if (D_80115520 == 0) {
        dx = D_80112E94 - o->pos.x;
        dz = D_80109DF0 - o->pos.z;
        dx = func_80069920(dz, dx);
        p = o; p->fwd.x = __cosf(dx);
        p = o; p->fwd.z = __sinf(dx);
        o->side.x = o->fwd.y * o->up.z - o->fwd.z * o->up.y;
        o->side.y = o->fwd.z * o->up.x - o->fwd.x * o->up.z;
        o->side.z = o->fwd.x * o->up.y - o->fwd.y * o->up.x;
    } else {
        o->u.walker.unk_EC.fn = func_8003086C;
        if (D_80115520 < 0) {
            func_800375A8(o, 0x16);
            func_80023308();
            func_8004BD1C(D_80133FB8, 0);
        }
    }
}
