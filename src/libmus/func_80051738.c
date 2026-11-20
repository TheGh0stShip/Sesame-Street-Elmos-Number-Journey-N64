#include "common.h"
#include "libmus/player.h"

/* parses a note event (skipping an optional extra byte when bit 7 is set) and calls a hook in mode 2 */
u8 *Fmarker(channel_t *s, u8 *p) {
    u8 a;
    u8 b;
    a = *p++; b = *p++; if (b & 0x80) { b = *p++; }
    if ((s->flags & 3) == 2 && (*(void (*(*))(s32, u8))&D_800A0580) != NULL) {
        (*(void (*(*))(s32, u8))&D_800A0580)(s->handle, a);
    }
    return p;
}
