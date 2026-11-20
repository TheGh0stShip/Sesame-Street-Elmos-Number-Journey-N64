#include "common.h"
#include "libmus/player.h"

/* Sets unk_30 = unk_90 + val on every object whose unk_44 equals id; returns how many. */
s32 MusHandleSetFreqOffset(s32 id, f32 val) {
    s32 n, i; channel_t *o;
    if (id == 0) {
        return 0;
    }
    n = 0;
    for (i = 0, o = mus_channels; i < max_channels; i++, o++) {
        if (o->handle == id) {
            o->freqoffset = val + o->distort;
            n++;
        }
    }
    return n;
}
