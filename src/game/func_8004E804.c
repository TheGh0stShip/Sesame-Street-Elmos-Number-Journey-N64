#include "common.h"
#include "libmus/player.h"

/* count active entries matching flag bit 1 (unk_78 set) or bit 2 (unk_78 clear) */
s32 MusAsk(s32 flags) {
    s32 i;
    s32 n;
    channel_t *e;
    i = 0; n = 0;
    e = mus_channels;
    while (i < max_channels) {
        if (e->pdata != 0) {
            if (e->fx_addr != 0) {
                if (flags & 1) goto inc;
            } else {
                if (flags & 2) {
inc:
                    n++;
                }
            }
        }
        i++;
        e++;
    }
    return n;
}
