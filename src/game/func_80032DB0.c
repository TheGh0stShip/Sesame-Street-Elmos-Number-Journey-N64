#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;

/* Camera update: builds the view matrices, or follows the player while easing in. */
void func_80032DB0(Actor *arg0) {
    register Actor *c = arg0;
    Mtx persp;
    Mtx look;
    Mtx view;
    register f32 dx;
    register f32 dz;
    register f32 s;
    register f64 d;

    if (D_800AD03A != 0) {
        guPerspectiveF(&persp, &D_800A7F00, 40.0f, 1.3333334f, 50.0f, 10000.0f, 1.0f);
        guLookAtF(&look, c->pos.x, c->pos.y, c->pos.z, c->fwd.x, c->fwd.y, c->fwd.z, c->up.x, c->up.y, c->up.z);
        guMtxCatF(&look, &persp, &view);
        guMtxF2L(&view, D_8013EF28);
        addActorDL(D_80032894, c, 1, 0, 0);
    } else {
        if (c->u.camera.timer != 0) {
            c->u.camera.timer--;
            c->fwd.x += c->side.x;
            c->fwd.y += c->side.y;
            c->fwd.z += c->side.z;
        }
        dx = D_80106C84->pos.x - c->fwd.x;
        dz = D_80106C84->pos.z - c->fwd.z;
        d = 844.0 - (f32)((0x80 - c->u.camera.timer) * 6);
        s = d / sqrtf(dx * dx + dz * dz);
        dx *= s;
        dz *= s;
        s = c->u.camera.timer;
        s *= 3.8125;
        c->pos.x = dx + D_80106C84->pos.x - (0x80 - c->u.camera.timer);
        c->pos.y = s + 25.0;
        c->pos.z = D_80106C84->pos.z + dz;
    }
}
