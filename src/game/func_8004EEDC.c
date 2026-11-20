#include "common.h"
#include "libmus/player.h"

/* looks up entry with id and returns its value from attached object */
s32 MusHandleWaveAddress(s32 id) {
    s32 i;
    channel_t *e;
    if (id == 0) return 0;
    e = mus_channels;
    for (i = 0; i < max_channels; i++, e++) {
        if (e->handle == id) {
            if (e->song_addr != NULL) return e->song_addr->WaveTable;
            return e->fx_addr->wave_table;
        }
    }
    return 0;
}
