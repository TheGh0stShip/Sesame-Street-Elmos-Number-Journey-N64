#include "common.h"
#include "libmus/player.h"

/* sequence player: push the current loop/return state onto the channel's stack */
u8 *Ffor(channel_t *c, u8 *p)
{
    u8 d = c->for_stack_count;
    c->for_count[d] = *p++;
    c->for_stack[d] = p;
    (*(s32 (*)[4])&c->for_stackvol)[d] = c->pvolume;
    (*(s32 (*)[4])&c->for_stackpb)[d] = c->ppitchbend;
    c->for_volume[d] = c->volume;
    c->for_pitchbend[d] = c->pitchbend;
    c->for_vol_count[d] = c->volume_count;
    c->for_pb_count[d] = c->pitchbend_count;
    c->for_stack_count++;
    return p;
}
