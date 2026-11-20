#include "common.h"
#include "game_structs.h"


/* aims the object at a target point and rebuilds its cross-product vector */
void func_80039E1C(Actor *arg0) {
    register f32 dx;
    register f32 dz;
    register Actor *o;

    if (D_80115520 == 0) {
        dx = D_80112E94 - arg0->pos.x;
        dz = D_80109DF0 - arg0->pos.z;
        dx = func_80069920(dz, dx);
        o = arg0;
        o->fwd.x = __cosf(dx);
        o = arg0;
        o->fwd.z = __sinf(dx);
        arg0->side.x = arg0->fwd.y * arg0->up.z - arg0->fwd.z * arg0->up.y;
        arg0->side.y = arg0->fwd.z * arg0->up.x - arg0->fwd.x * arg0->up.z;
        arg0->side.z = arg0->fwd.x * arg0->up.y - arg0->fwd.y * arg0->up.x;
    } else if (D_80115520 < 0) {
        func_800375A8(arg0, 0);
        func_80023308();
        func_8004BD1C(D_80133FB8, 0);
    }
}
