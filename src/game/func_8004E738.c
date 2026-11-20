#include "common.h"
#include "libmus/player.h"

/* size 0x13C */



/* for objects selected by mask bits (1: with unk_78 set, 2: without) that are active and have unk_10 == -1, assign arg */
void MusStop(s32 mask, s32 arg) {
    s32 v;
    s32 i;
    channel_t *o;

    v = arg;
    if (arg == 0) {
        v = 1;
    }
    o = mus_channels;
    for (i = 0; i < max_channels; i++, o++) {
        if ((o->fx_addr != 0) ? !(mask & 1) : !(mask & 2)) {
            continue;
        }
        if (o->pdata != 0 && o->stopping == -1) {
            if (o->flags & 1) {
                o->stopping_speed = 1;
                o->stopping = 0;
                o->flags &= ~1;
            } else {
                o->stopping_speed = v;
                o->stopping = arg;
            }
        }
    }
}
