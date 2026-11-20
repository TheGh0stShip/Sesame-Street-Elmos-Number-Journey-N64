#include "common.h"
#include "libmus/player.h"

/* Sets unk_9E on every object whose unk_44 equals id; returns how many. */
s32 MusHandleSetVolume(s32 id, s16 val) {
    s32 n, i; channel_t *o;
    if (id == 0) {
        return 0;
    }
    n = 0;
    for (i = 0, o = mus_channels; i < max_channels; i++, o++) {
        if (o->handle == id) {
            o->volscale = val;
            n++;
        }
    }
    return n;
}
