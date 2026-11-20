#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;
/* sqrt wrapper is a statement-expression macro returning double */
#define SQRT(x) ({ (f64)sqrtf(x); })

/* camera update: builds view matrices, or moves the camera to follow the player */
void func_80032B28(Actor *arg0) {
    register Actor *c = arg0;
    register f32 dx;
    register f32 dz;
    register f32 s;
    f32 persp[4][4];
    f32 view[4][4];
    f32 mtx[4][4];

    if (D_800AD03A != 0) {
        guPerspectiveF(persp, &D_800A7F00, 40.0f, 1.3333334f, 50.0f, 10000.0f, 1.0f);
        guLookAtF(view, c->pos.x, c->pos.y, c->pos.z, c->fwd.x, c->fwd.y, c->fwd.z, c->up.x, c->up.y, c->up.z);
        guMtxCatF(view, persp, mtx);
        guMtxF2L(mtx, D_8013EF28);
        addActorDL(func_80032894, c, 1, 0, 0);
        return;
    }
    if (c->u.camera.timer != 0) {
        c->u.camera.timer--;
        c->fwd.x += c->side.x;
        c->fwd.y += c->side.y;
        c->fwd.z += c->side.z;
    }
    dx = D_80106C84->pos.x - c->fwd.x;
    dz = D_80106C84->pos.z - c->fwd.z;
    s = ((f32)(c->u.camera.timer * 2) + 844.0) / SQRT(dx * dx + dz * dz);
    dx *= s;
    dz *= s;
    s = 0;
    s = 0x80 - c->u.camera.timer;
    s = s * 2.25;
    c->pos.x = D_80106C84->pos.x + dx;
    c->pos.y = s + 225.0;
    c->pos.z = D_80106C84->pos.z + dz;
}
