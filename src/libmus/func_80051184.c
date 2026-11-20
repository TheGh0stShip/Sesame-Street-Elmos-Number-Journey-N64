#include "common.h"
#include "libmus/player.h"

u8 *Flength(channel_t *p, u8 *ptr) {
    s32 v = *ptr++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *ptr++;
    }
    p->fixed_length = v;
    return ptr;
}
