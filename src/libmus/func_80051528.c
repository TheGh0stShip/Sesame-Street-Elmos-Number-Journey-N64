#include "common.h"
#include "libmus/player.h"

/* sequence player: read a mapped byte plus an offset byte into a channel field */
u8 *FrandVolume(channel_t *c, u8 *p)
{
    u8 v = func_800504E8(*p++);
    c->volume = v;
    c->volume = v + *p++;
    return p;
}
