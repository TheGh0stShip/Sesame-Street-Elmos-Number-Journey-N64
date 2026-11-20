#include "common.h"
#include "libmus/player.h"

u8 *Fcutoff(channel_t *p, u8 *s)
{
    u16 hi = *s++;
    u16 lo = *s++;
    p->endit = 0;
    p->cutoff = (hi << 8) | lo;
    return s;
}
