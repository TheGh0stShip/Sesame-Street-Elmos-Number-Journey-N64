#include "common.h"
#include "libmus/player.h"

/* size 0x13C */



/* sets unk_B0 and marks unk_BE on every object whose unk_44 equals id; returns how many */
s32 MusHandleSetPan(s32 id, s16 val) {
    s32 i;
    s32 n;
    channel_t *o;

    if (id == 0) {
        return 0;
    }
    n = 0;
    o = mus_channels;
    for (i = 0; i < max_channels; i++, o++) {
        if (o->handle == id) {
            n++;
            o->panscale = val;
            o->old_pan = 0xFF;
        }
    }
    return n;
}
