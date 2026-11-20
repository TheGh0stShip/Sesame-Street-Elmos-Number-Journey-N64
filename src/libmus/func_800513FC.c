#include "common.h"
#include "libmus/player.h"

/* reads 1-2 byte index, points unk_84 at entry of 6-byte table */
u8 *Fdrums(channel_t *p, u8 *ptr) {
    s32 v = *ptr++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *ptr++;
    }
    p->pdrums = &p->song_addr->DrumData[v];
    return ptr;
}
