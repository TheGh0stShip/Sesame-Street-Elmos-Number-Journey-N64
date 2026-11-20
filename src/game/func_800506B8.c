#include "common.h"
#include "libmus/player.h"


/* size 0x13C */


extern channel_t *D_800A0550;

/* picks an object slot for an owner: a free slot, else the oldest, else one already tied to the owner */
s32 func_800506B8(song_t *owner, s32 idx) {
    s32 i;
    s32 best;
    s32 sel;
    channel_t *o;

    if (idx < 0) {
        o = mus_channels;
        for (i = 0; i < 4; i++, o++) {
            if (o->pdata == 0) {
                return i;
            }
        }
    }
    for (i = 4, o = D_800A0550; i < max_channels; i++, o++) {
        if (o->pdata == 0) {
            return i;
        }
    }
    i = 4;
    o = D_800A0550;
    best = 0x7FFFFFFF;
    sel = 3;
    for (; i < max_channels; i++, o++) {
        if (o->fx_addr != 0 && o->priority <= best) {
            best = o->priority;
            sel = i;
        }
    }
    if (sel >= 4) {
        return sel;
    }
    for (i = 4, o = D_800A0550; i < max_channels; i++) {
        if (o->fx_addr == 0 && o->song_addr != owner) {
            return i;
        }
    }
    for (i = 4, o = D_800A0550; i < max_channels; i++, o++) {
        if (o->song_addr == owner && (*(s32 * *)&owner->ChannelData)[idx] == o->pbase) {
            return i;
        }
    }
    return idx % (max_channels - 4) + 4;
}
