#include "common.h"
#include "game_structs.h"

extern Actor *D_80106C84;
void func_8000F8D8(void);

/* periodically spawn a randomised particle at an offset rotated by the actor's facing */
void func_8000FDB8(s32 x, s32 y, s32 z) {
    register Actor *a;
    register Particle *p;
    register u32 r;
    register u32 t;
    register f32 s;
    register f32 c;
    register f32 fx;
    register f32 fz;
    register f32 nx;
    s32 pad;

    a = D_80106C84;
    if (a->u.model.unk_1F0.l1.unk_1F3 > 0) {
        if (!((*(u32 (*))&D_80106C7C) & 1)) {
            a->u.model.unk_1F0.l1.unk_1F3--;
        }
    } else if (a->u.model.unk_1F0.l1.unk_1F3 == 0) {
        a->u.model.unk_1F0.l1.unk_1F3 = (func_80021640() & 7) - 15;
    } else if ((*(u32 (*))&D_80106C7C) % 3 == 0) {
        if (++a->u.model.unk_1F0.l1.unk_1F3 == 0) {
            a->u.model.unk_1F0.l1.unk_1F3 = (func_80021640() & 0x1F) + 50;
        }
        if ((p = func_80012E78()) != NULL) {
            p->unk_8 = func_8000F8D8;
            s = a->fwd.x;
            c = a->fwd.z;
            fx = x;
            fz = z;
            nx = c * fx + s * fz;
            fz = fz * c - s * fx;
            x = nx;
            z = fz;
            { register s16 i = x + a->pos.x; p->x.h = i << 2; }
            { register s16 i = y + a->pos.y; p->y.h = i << 2; }
            { register s16 i = z + a->pos.z; p->z.h = i << 2; }
            r = func_80021640();
            t = r & 0x1F;
            t += 18;
            r >>= 5;
            p->life = t;
            t = r & 0x1F;
            r >>= 5;
            p->timer.b = t;
            t = r & 0x1F;
            r >>= 5;
            p->vx = t;
            p->u_1C.s.unk_1C.b = 75;
        }
    }
}
