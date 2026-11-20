#include "common.h"
#include "libmus/player.h"

extern Syn *D_800A0548;
void func_80055E50(Syn *, s16, s32);
void func_80055D30(Syn *, u8);
/* update channel volume and pan */
void func_8004F990(channel_t *c, s32 idx) {
    u32 v;
    v = (u32)(c->volume * c->env_current * c->velocity * c->volscale) >> 13;
    if (v > 0x7FFF) v = 0x7FFF;
    if (c->fx_addr == 0) v *= D_800A055E; else v *= D_800A055C;
    v >>= 15;
    if (c->stopping != -1) v = v * c->stopping / c->stopping_speed;
    if (v != c->old_volume) {
        c->old_volume = v;
        func_80055E50(&D_800A0548[idx], v, D_800A0558);
    }
    v = ((c->pan * c->panscale) >> 7) & 0x7F;
    if (v != c->old_pan) {
        c->old_pan = v;
        func_80055D30(&D_800A0548[idx], v);
    }
}
