#include "common.h"

typedef union {
    s32 w;
    struct {
        u16 hi;
        u16 lo;
    } h;
} Fix;
typedef struct {
    u8 pad0[0xC];
    Fix x;
    Fix y;
    Fix z;
    s8 timer;
    s8 vx;
    s8 vy;
    s8 vz;
    u8 pad1C[0x8];
    u16 size;
} Spark;
typedef struct {
    u8 pad0[0x24];
    f32 x;
    f32 y;
    f32 z;
} Cam;
extern Cam *D_80106C84;
extern Vtx *D_80128578;

#define GFX(a, b) { Gfx *_g = (Gfx *)(D_8013EC7C++); _g->words.w0 = (a); _g->words.w1 = (b); }

/* moves a spark, then draws it as a camera-facing triangle coloured by its remaining life */
void func_80010028(register Spark *s) {
    register Cam *cam;
    register s16 x;
    register s16 y;
    register s16 z;
    register s32 dx;
    register s32 dz;
    register s32 d;
    register s32 t;
    register f32 len;

    if (--s->timer == 0) {
        func_80012F14(s);
        return;
    }
    cam = D_80106C84;
    s->x.w += s->vx << 16;
    s->y.w += s->vy << 16;
    s->z.w += s->vz << 16;
    x = cam->x;
    x <<= 2;
    y = cam->y;
    y <<= 2;
    z = cam->z;
    z <<= 2;
    x += s->x.h.hi;
    z += s->z.h.hi;
    y += s->y.h.hi;
    dx = D_800BD87C;
    dx <<= 2;
    dx -= x;
    dz = D_80106C6E;
    dz <<= 2;
    dz -= z;
    d = dx * dx;
    t = dz * dz;
    d += t;
    if (d < 0) {
        return;
    }
    len = sqrtf(d);
    d = s->size;
    if (len != 0.0f) {
        len = d / len;
        dx = dx * len;
        dz = dz * len;
        GFX(0x04000C2F, (u32)D_80128578);
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
        GFX(0xE7000000, 0);
        t = func_80021640();
        dx = t & 7;
        if (dx) {
            if (s->timer >= 9) {
                GFX(0xFA000000, 0xFFFF00FF);
                GFX(0xFB000000, 0xFFFF0000);
            } else if (s->timer >= 7) {
                GFX(0xFA000000, 0xFFFF00FF);
                GFX(0xFB000000, 0xFF800000);
            } else if (s->timer >= 4) {
                GFX(0xFA000000, 0xFF8000FF);
                GFX(0xFB000000, 0xFF000000);
            } else {
                GFX(0xFA000000, 0xFF0000FF);
                GFX(0xFB000000, 0xFF000000);
            }
        } else {
            GFX(0xFA000000, 0xFFFFFFFF);
            GFX(0xFB000000, 0xFFFFFF00);
        }
        GFX(0xBB000001, 0x80008000);
        GFX(0xBF000000, 0x204);
    }
}
