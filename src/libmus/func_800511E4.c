#include "common.h"
#include "libmus/player.h"

u8 *Fdistort(channel_t *p, u8 *ptr) {
    s32 v = *ptr++;
    f32 f;
    if (v & 0x80) v |= 0xFFFFFF00;
    f = (f32)v / 100.0;
    p->freqoffset = p->freqoffset - p->distort + f;
    p->distort = f;
    return ptr;
}
