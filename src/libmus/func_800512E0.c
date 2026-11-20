#include "common.h"
#include "libmus/player.h"

/* pop/repeat a loop stack entry */
s32 Fnext(channel_t *p, s32 ret) {
    s32 i = p->for_stack_count - 1;
    if (p->for_count[i] != 0xFF) {
        if (--p->for_count[i] == 0) {
            p->for_stack_count = i;
            i = -1;
        }
    }
    if (i >= 0) {
        ret = (*(s32 (*)[4])&p->for_stack)[i];
        p->pvolume = (*(s32 (*)[4])&p->for_stackvol)[i];
        p->ppitchbend = (*(s32 (*)[4])&p->for_stackpb)[i];
        p->volume = p->for_volume[i];
        p->pitchbend = p->for_pitchbend[i];
        p->volume_count = p->for_vol_count[i];
        p->pitchbend_count = p->for_pb_count[i];
        p->pitchbend_precalc = p->pitchbend * p->bendrange;
    }
    return ret;
}
