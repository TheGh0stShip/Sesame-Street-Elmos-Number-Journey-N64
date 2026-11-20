#include "common.h"
#include "libmus/player.h"

/* find entry with id arg0, return its unk_7C */
s32 MusHandleGetPtrAddr(s32 id) {
    s32 i;
    channel_t *e;
    if (id == 0) return 0;
    e = mus_channels;
    for (i = 0; i < max_channels; i++, e++) {
        if (e->handle == id) return e->sample_bank;
    }
    return 0;
}
