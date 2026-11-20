#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;
/* turns the object toward an angle: wraps the difference to [-pi, pi] and sets velocity from speed */
void func_8002F640(register f32 ang) {
    register Actor *o = D_80106C84;
    register f32 d;
    d = func_80069920(-o->u.walker.unk_1A8.l0.scale.y, -o->u.walker.unk_1A8.l1.unk_1AC.z);
    d -= ang;
    if (d > 3.14159) {
        d = d - 6.2831853;
    } else if (d < -3.14159) {
        d = d + 6.2831853;
    }
    ang -= d;
    o->u.walker.speed = __sinf(ang) * o->u.walker.offset.v.y;
    o->u.walker.unk_1E8 = __cosf(ang) * o->u.walker.offset.v.y;
    o->u.walker.offset.v.y = 0;
    if (d >= 0) {
        o->u.walker.offset.v.z = 0.15f;
    } else {
        o->u.walker.offset.v.z = -0.15f;
    }
}
