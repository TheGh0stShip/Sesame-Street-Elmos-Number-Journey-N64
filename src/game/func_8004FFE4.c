#include "common.h"
#include "libmus/player.h"

/* advances a byte-coded note sequence up to the current time */
void func_8004FFE4(channel_t *s) {
    u8 b;
    do {
        (*(s32 *)&s->volume_frame) += 0x100;
        if (--s->volume_count == 0) {
            b = *s->pvolume++;
            if (b >= 0x80) {
                s->volume = b & 0x7F;
                b = *s->pvolume++;
                if (b >= 0x80) {
                    s->volume_count = (b & 0x7F) << 8;
                    s->volume_count += *s->pvolume++ + 2;
                } else {
                    s->volume_count = b + 2;
                }
            } else {
                s->volume = b;
                s->volume_count = 1;
            }
        }
    } while ((*(s32 *)&s->volume_frame) - (*(s32 *)&s->channel_frame) < 0);
}
