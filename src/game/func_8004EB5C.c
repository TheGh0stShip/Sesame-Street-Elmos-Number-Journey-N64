#include "common.h"
#include "libmus/player.h"

/* sets value (clamped 0..127) on all entries with id, marks them; returns count */
s32 MusHandleSetReverb(s32 id, s32 val) {
    s32 i, n;
    channel_t *e;
    if (id == 0) return 0;
    if (val < 0) val = 0;
    else if (val >= 0x80) val = 0x7F;
    n = 0;
    e = mus_channels;
    for (i = 0; i < max_channels; i++, e++) {
        if (e->handle == id) {
            n++;
            e->reverb_base = val;
            e->old_reverb = 0xFF;
        }
    }
    return n;
}
