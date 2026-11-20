#include "common.h"
#include "game_structs.h"


/* allocates an object at (x, y, z) * 4 with randomised fields */
void func_80011468(s32 arg0, s32 arg1, s32 arg2) {
    register Particle *obj;
    register u32 r;
    register s32 t;
    s8 pad0;

    if ((obj = func_80012E78()) != NULL) {
        obj->unk_8 = &D_80010E40;
        obj->x.h = ((u16)arg0) << 2;
        obj->y.h = ((u16)arg1) << 2;
        obj->z.h = ((u16)arg2) << 2;
        r = func_80021640();
        t = r & 15;
        t = t + 60;
        r = r >> 4;
        obj->life = t;
        obj->u_1C.s.unk_1C.h = ((u16)arg1) << 2;
        obj->timer.b = 0;
        t = r & 127;
        t = t - 64;
        r = r >> 7;
        t = t & 248;
        obj->vx = t;
        t = r & 63;
        t = t + 64;
        r = r >> 7;
        obj->vy.b = t;
        t = r & 127;
        t = t - 64;
        r = r >> 7;
        t = t & 248;
        obj->vz = t;
    }
}
