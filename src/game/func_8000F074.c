#include "common.h"
#include "game_structs.h"

extern Vtx *D_80128578;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define gDPSetColor(pkt, cmd, r, g, b, a) \
    { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (cmd); \
      _g->words.w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); }
#define gDPSetPrimColor(pkt, r, g, b, a) gDPSetColor(pkt, 0xFA000000, r, g, b, a)
#define gDPSetEnvColor(pkt, r, g, b, a) gDPSetColor(pkt, 0xFB000000, r, g, b, a)
#define gDPWords(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (b); }

/* moves a spark particle and draws it as a camera-facing triangle with a colour chosen by p->color */
void func_8000F074(register Particle *p) {
    register u16 x;
    register u16 y;
    register u16 z;
    register s32 dx;
    register s32 dz;
    register s32 d;
    register s32 t;
    register f32 len;

    if (!(D_80106C7C & 1)) {
        if (--p->life < 20) {
            func_80012F14(p);
        }
    }
    p->x.fix.w += p->vx << 9;
    p->y.fix.w -= p->vy.ub << 11;
    p->z.fix.w += p->vz << 9;
    x = p->x.fix.h;
    z = p->z.fix.h;
    y = p->y.fix.h;
    dx = (D_800BD87C << 2) - (s16)x;
    dz = (D_80106C6E << 2) - (s16)z;
    d = dx * dx;
    t = dz * dz;
    d = d + t;
    if (d < 0) {
        return;
    }
    len = sqrtf(d);
    d = p->life;
    if (len != 0.0f) {
    len = d / len;
    dx = dx * len;
    dz = dz * len;
    gDPWords(D_8013EC7C++, 0x04000C2F, (u32)D_80128578);
    D_80128578->v.ob[0] = x;
    D_80128578->v.ob[1] = y - d;
    D_80128578->v.ob[2] = z;
    D_80128578->v.tc[0] = 0x400;
    D_80128578->v.tc[1] = 0xC00;
    d = d >> 1;
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
    gDPWords(D_8013EC7C++, 0xE7000000, 0);
    if (--p->u_1C.at1.flicker == 0) {
        dx = func_80021640();
        dz = (dx & 7) + 4;
        p->u_1C.at1.flicker = dz;
        dx = 0xFF;
    } else {
        dx = 0xC8;
    }
    switch (p->u_1C.s.unk_1C.color) {
        case 0:
            gDPSetPrimColor(D_8013EC7C++, dx, 0, 0, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, dx, 0, 0, 0);
            break;
        case 1:
            gDPSetPrimColor(D_8013EC7C++, 0, dx, 0, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, 0, dx, 0, 0);
            break;
        case 2:
            gDPSetPrimColor(D_8013EC7C++, 0, 0, dx, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, 0, 0, dx, 0);
            break;
        case 3:
            gDPSetPrimColor(D_8013EC7C++, dx, dx, 0, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, dx, dx, 0, 0);
            break;
        case 4:
            gDPSetPrimColor(D_8013EC7C++, 0, dx, dx, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, 0, dx, dx, 0);
            break;
        case 5:
            gDPSetPrimColor(D_8013EC7C++, dx, 0, dx, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, dx, 0, dx, 0);
            break;
        case 6:
            gDPSetPrimColor(D_8013EC7C++, dx, dx, dx, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, dx, dx, dx, 0);
            break;
        case 7:
            gDPSetPrimColor(D_8013EC7C++, dx, 0x80, 0x80, 0xFF);
            gDPSetEnvColor(D_8013EC7C++, dx, dx >> 1, dx >> 1, 0);
            break;
    }
    gDPWords(D_8013EC7C++, 0xBB000501, 0x80008000);
    gDPWords(D_8013EC7C++, 0xBF000000, 0x204);
    }
}
