#include "common.h"
#include "game_structs.h"

/* actor update: timed sound/scene events, then turns its facing toward a target direction */

void func_8003B6D8(Actor *arg0) {
    register Actor *a;
    register f32 cur;
    register f32 diff;

    if (arg0->u.walker.offset.s == 25) {
        if (arg0->unk_C.l1.flags & 8) {
            func_80037450(arg0, 0);
            arg0->u.walker.goalDir.x = 0.0f;
            arg0->u.walker.goalDir.z = -1.0f;
            arg0->u.walker.frame = 0;
        }
        return;
    }
    if (arg0->u.walker.frame == 60 && D_8011551C == NULL) {
        D_8011551C = &D_80074CFC;
        osSendMesg(&D_8013EEF8, 0, 1);
    }
    if (arg0->u.walker.frame >= 721 && arg0->u.walker.frame < 780 && D_8011551C == NULL) {
        func_80018CB0();
    }
    if (arg0->u.walker.frame >= 781 && D_8011551C == NULL) {
        func_800375A8(arg0, 0);
        arg0->u.walker.unk_EC.fn = &D_800319A0;
    }
    cur = func_80069920(arg0->fwd.x, arg0->fwd.z);
    diff = func_80069920(arg0->u.walker.goalDir.x, arg0->u.walker.goalDir.z);
    diff = diff - cur;
    if (diff > 3.14159) {
        diff = diff - 6.2831853;
    } else if (diff < -3.14159) {
        diff = diff + 6.2831853;
    }
    if (fabs(diff) < 0.03) {
        arg0->fwd.x = arg0->u.walker.goalDir.x;
        arg0->fwd.y = arg0->u.walker.goalDir.y;
        arg0->fwd.z = arg0->u.walker.goalDir.z;
        if (arg0->u.walker.unk_1EC > 0) {
            arg0->u.walker.unk_1EC = -1;
        }
    } else {
        cur = cur + diff * 0.2;
        a = arg0;
        a->fwd.x = __sinf(cur);
        a = arg0;
        a->fwd.z = __cosf(cur);
        arg0->u.walker.turning = 1;
        arg0->u.walker.unk_1A8.l0.scale.y = 5.0f;
        arg0->u.walker.unk_1A8.l0.unk_1B4 = 0;
    }
    arg0->side.x = arg0->fwd.y * arg0->up.z - arg0->fwd.z * arg0->up.y;
    arg0->side.y = arg0->fwd.z * arg0->up.x - arg0->fwd.x * arg0->up.z;
    arg0->side.z = arg0->fwd.x * arg0->up.y - arg0->fwd.y * arg0->up.x;
}
