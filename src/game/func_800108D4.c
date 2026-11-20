#include "common.h"
#include "game_structs.h"

#define GW(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (b); }

extern Vtx *D_80128578;
/* moves a falling particle and draws it as a camera-facing triangle */
void func_800108D4(register Particle *o) {
    register s16 x, y, z;
    register s32 dx, dz, d, e;
    register f32 len;

    o->x.s += o->vx << 13;
    o->y.s += o->vy.b << 16;
    o->z.s += o->vz << 13;
    if (o->vy.b >= -0x76) o->vy.b -= 8;
    x = *(s16 *)&o->x.s;
    y = *(s16 *)&o->y.s;
    z = *(s16 *)&o->z.s;
    if (o->u_1C.s.unk_1C.h > y) {
        func_80012F14(o);
        return;
    }
    dx = D_800BD87C;
    dx <<= 2;
    dx -= x;
    dz = D_80106C6E;
    dz <<= 2;
    dz -= z;
    d = dx * dx;
    e = dz * dz;
    d += e;
    if (d < 0) return;
    len = sqrtf(d);
    d = o->life;
    if (len != 0.0f) {
    len = d / len;
    dx = dx * len;
    dz = dz * len;
    GW(D_8013EC7C++, 0x04000C2F, (u32)D_80128578);
    D_80128578->v.ob[0] = x;
    D_80128578->v.ob[1] = y - d;
    D_80128578->v.ob[2] = z;
    D_80128578->v.tc[0] = 0x400;
    D_80128578->v.tc[1] = 0xC00;
    d >>= 1;
    D_80128578++;
    D_80128578->v.ob[0] = x + dz;
    D_80128578->v.ob[1] = y + d;
    D_80128578->v.ob[2] = z - dx;
    D_80128578->v.tc[0] = -0x400;
    D_80128578->v.tc[1] = 0;
    D_80128578++;
    D_80128578->v.ob[0] = x - dz;
    D_80128578->v.ob[1] = y + d;
    D_80128578->v.ob[2] = z + dx;
    D_80128578->v.tc[0] = 0xC00;
    D_80128578->v.tc[1] = 0;
    D_80128578++;
    GW(D_8013EC7C++, 0xE7000000, 0);
    dx = ++o->timer.ub;
    dx <<= 3;
    dx += 0x30;
    if (dx > 0xFF) dx = 0xFF;
    GW(D_8013EC7C++, 0xFA000000, ((dx & 0xFF) << 24) | ((dx & 0xFF) << 16) | 0xFFFF);
    GW(D_8013EC7C++, 0xFB000000, 0xFFFFFF00);
    GW(D_8013EC7C++, 0xBB000001, 0x80008000);
    GW(D_8013EC7C++, 0xBF000000, 0x204);
    }
}
