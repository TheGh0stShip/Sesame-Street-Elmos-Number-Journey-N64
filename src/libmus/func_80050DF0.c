#include "common.h"
#include "libmus/player.h"

/* sequence player: read a 7/15-bit variable-length value into a channel field */
u8 *Fwave(channel_t *c, u8 *p)
{
    u32 v = *p++;
    if (v & 0x80) {
        v = *p++ | ((v & 0x7F) << 8);
    }
    c->wave = v;
    return p;
}
