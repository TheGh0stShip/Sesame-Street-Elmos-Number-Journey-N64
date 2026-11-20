#include "common.h"
#include "libmus/player.h"

typedef struct { u8 unk_00[0x1C]; } Chan;
extern Chan *D_800A0548;
void func_80055DC0(Chan *, f32);
/* updates a voice's pitch (with glide) and sends the ratio to its channel */
void func_8004FAE4(channel_t *v, s32 idx, f32 bend) {
    f32 p = v->base_note;
    if (v->port != 0) {
        if (v->port >= v->ticks) {
            p = v->port_base + (p - v->port_base) / v->port * v->ticks;
        }
        v->port_current = p;
    }
    p += bend + v->pitchbend_precalc;
    if (p != v->old_frequency) {
        v->old_frequency = p;
        p = func_800501BC(p * 0.08333333333333333);
        if (p > 2.0) {
            p = 2.0f;
            v->velocity = 0;
        }
        func_80055DC0(&D_800A0548[idx], p);
    }
}
