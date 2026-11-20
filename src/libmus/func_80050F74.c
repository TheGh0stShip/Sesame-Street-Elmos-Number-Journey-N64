#include "common.h"
#include "libmus/player.h"

/* Sequence tempo command: sets tempo from a byte and propagates it to matching channels */

u8 *Ftempo(channel_t *p, u8 *cmd) {
    s32 tempo;
    s32 vol;
    s32 i;

    tempo = ((*cmd++ * 3) << 13) / 120 / D_800A0554;
    vol = (tempo * p->temscale) >> 7;
    if (p->fx_addr != 0) {
        p->channel_tempo = tempo;
    } else {
        channel_t *c = mus_channels;
        for (i = 0; i < max_channels; i++, c++) {
            if (c->song_addr == p->song_addr) {
                c->tempo = tempo;
                c->channel_tempo = vol;
            }
        }
    }
    return cmd;
}
