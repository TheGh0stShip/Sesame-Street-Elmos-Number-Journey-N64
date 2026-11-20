#include "common.h"
#include "libmus/player.h"

/* read a 1-2 byte index from the stream, create an object for it and retarget matching entries */

u8 *Fstartfx(channel_t *obj, u8 *p)
{
    s32 n;
    s32 r;
    s32 i;

    n = *p++;
    if (n >= 0x80)
        n = ((n & 0x7F) << 8) + *p++;
    obj->priority++;
    r = func_80050968(obj->fx_addr, n, obj->volscale, obj->panscale, obj->priority);
    obj->priority--;
    if (r != 0) {
        channel_t *e = mus_channels;
        for (i = 0; i < max_channels; i++, e++) {
            if (e->handle == r) {
                e->handle = obj->handle;
                e->sample_bank = obj->sample_bank;
            }
        }
    }
    return p;
}
