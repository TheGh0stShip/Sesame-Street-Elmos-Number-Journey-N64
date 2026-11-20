#include "common.h"
#include "libmus/player.h"

/* set volume-like value on all entries with id; returns count */
s32 MusHandleSetTempo(s32 id, s32 v) {
    s32 i, n;
    channel_t *e;
    if (id == 0) return 0;
    if (v <= 0) v = 1;
    else if (v > 0x100) v = 0x100;
    n = 0;
    e = mus_channels;
    for (i = 0; i < max_channels; i++, e++) {
        if ((*(s32 *)&e->handle) == id) {
            n++;
            e->temscale = v;
            e->channel_tempo = ((*(u16 *)&e->tempo) * v) >> 7;
        }
    }
    return n;
}
