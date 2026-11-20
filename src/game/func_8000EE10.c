#include "common.h"
#include "game_structs.h"

void func_8000F074(void);

/* spawns six objects with randomised position, size and colour fields */
void func_8000EE10(Actor *arg0) {
    register u32 r;
    register u32 t;
    register s32 n;
    register Actor *p;
    register Particle *o;

    p = arg0;
    n = 6;
    for (;;) {

        if ((o = func_80012E78()) != NULL) {
            o->unk_8 = func_8000F074;
            r = func_80021640();
            t = ((r & 0x3F) + 16) * p->fwd.x;
            r >>= 6;
            o->vx = t;
            o->x.h = (s16)((s16)p->pos.x + (s16)t) << 2;
            t = r & 0x3F;
            t += t;
            t += t;
            r >>= 6;
            o->y.h = (s16)((s16)p->pos.y + (s16)t) << 2;
            t = ((r & 0x3F) + 16) * p->fwd.z;
            r >>= 6;
            o->vz = t;
            o->z.h = (s16)((s16)p->pos.z + (s16)t) << 2;
            t = r & 0xF;
            r >>= 4;
            o->life = t + 60;
            t = r & 7;
            r >>= 3;
            o->u_1C.s.unk_1C.color = t;
            t = (r & 7) + 4;
            r >>= 3;
            o->u_1C.at1.flicker = t;
            t = ((r & 0xF) << 3) + 127;
            r >>= 4;
            o->vy.ub = t;
        }
        if (--n == 0) {
            break;
        }
    }
}
