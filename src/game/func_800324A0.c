#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;

/* eases angle 0x1E0 toward target 0x1E8 (wrapping at pi), then rebuilds a direction and cross product */
void func_800324A0(void) {
    register Actor *p;
    register f32 angle;
    register f32 diff;

    p = D_80106C84;
    if (p == NULL) {
        return;
    }
    angle = p->u.walker.speed;
    diff = p->u.walker.unk_1E8;
    diff -= angle;
    if (((diff >= 0.0f) ? diff : -diff) < 0.01) {
        angle = p->u.walker.unk_1E8;
        p->u.walker.unk_EC.s = 0;
    } else {
        if (p->u.walker.offset.v.z < 0.07) {
            p->u.walker.offset.v.z = p->u.walker.offset.v.z + 0.01;
        }
        if (diff > 3.14159) {
            diff = diff - 6.2831853;
        } else if (diff < -3.14159) {
            diff = diff + 6.2831853;
        }
        angle = angle + diff * p->u.walker.offset.v.z;
    }
    p->u.walker.speed = angle;
    p->fwd.x = __sinf(angle);
    p->fwd.z = __cosf(angle);
    p->side.x = p->fwd.y * p->up.z - p->fwd.z * p->up.y;
    p->side.y = p->fwd.z * p->up.x - p->fwd.x * p->up.z;
    p->side.z = p->fwd.x * p->up.y - p->fwd.y * p->up.x;
}
