#include "common.h"
#include "game_structs.h"

typedef struct { u8 pad0[0x30]; f32 unk_30; u8 pad34[4]; f32 unk_38; } Camera;
extern Camera *D_80106C84;
void func_80010028(void);
/* spawns 6..9 randomised particles at (x, y, z) rotated by the camera */
void func_800106BC(x, y, z)
    s32 x;
    s32 y;
    s32 z;
{
    register Camera *cam = D_80106C84;
    register Particle *p;
    register s32 n;
    register u32 r;
    register s32 t;
    register s32 px;
    register s32 pz;
    register f32 s;
    register f32 c;
    register f32 fx;
    register f32 fz;
    register f32 a;
    s32 i;

    n = (func_80021640() & 3) + 6;
    do {
        if ((p = func_80012E78()) != NULL) {
            p->unk_8 = func_80010028;
            s = cam->unk_30;
            c = cam->unk_38;
            fx = x;
            fz = z;
            a = c * fx + s * fz;
            fz = fz * c - s * fx;
            px = a;
            pz = fz;
            p->x.h = px * 4;
            p->y.h = (u16)y << 2;
            p->z.h = pz * 4;
            r = func_80021640();
            t = (r & 7) + 20;
            r >>= 3;
            p->life = t;
            t = (r & 7) + 8;
            r >>= 3;
            p->timer.b = t;
            t = (r & 0x1F) + 0x3C;
            r >>= 5;
            c = -t;
            t = (r & 0x1F) - 0xF;
            r >>= 5;
            s = (f32)t * 1.5;
            fx = c * cam->unk_30 + s * cam->unk_38;
            p->vx = fx;
            t = (r & 0x1F) - 0xF;
            r >>= 5;
            p->vy.b = t;
            fz = c * cam->unk_38 + s * cam->unk_30;
            p->vz = fz;
        }
    } while (--n);
}
