#include "common.h"
#include "game_structs.h"


/* turns obj toward a target point and rebuilds a cross product, or ends on negative state */
void func_80039270(Actor *obj) {
    register f32 a;
    register f32 b;

    if (D_80115520 == 0) {
        a = D_80112E94 - obj->pos.x;
        b = D_80109DF0 - obj->pos.z;
        a = func_80069920(b, a);
        obj->fwd.x = (a = a, __cosf(a));
        obj->fwd.z = (a = a, __sinf(a));
        obj->side.x = obj->fwd.y * obj->up.z - obj->fwd.z * obj->up.y;
        obj->side.y = obj->fwd.z * obj->up.x - obj->fwd.x * obj->up.z;
        obj->side.z = obj->fwd.x * obj->up.y - obj->fwd.y * obj->up.x;
    } else if (D_80115520 < 0) {
        func_800375A8(obj, 0xE);
        func_80023308();
        func_8004BD1C(D_80133FB8, 0);
    }
}
