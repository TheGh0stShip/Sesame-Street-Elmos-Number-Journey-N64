#include "common.h"
#include "libmus/player.h"

/* counts entries whose unk_44 equals key */
s32 MusHandleAsk(s32 key) {
    channel_t *e;
    s32 n;
    s32 i;
    s32 cnt;
    if (key == 0) {
        return 0;
    }
    i = 0;
    cnt = max_channels;
    e = mus_channels;
    n = 0;
    while (i < cnt) {
        n += e->handle == key;
        i++;
        e++;
    }
    return n;
}
