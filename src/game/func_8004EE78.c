#include "common.h"
#include "libmus/player.h"


/* Finds the entry whose unk_44 equals id and returns unk_8 of its unk_74 (or unk_78 if NULL). */
s32 MusHandleWaveCount(s32 id) {
    s32 i;
    fx_t *s;
    channel_t *e;
    s32 n;

    if (id != 0) {
        i = 0;
        n = max_channels;
        e = mus_channels;
        /* empty asm: an extra use of the table pseudo so global alloc ranks it above the count */
        __asm__("" :: "r"(e));
        if (i < n) {
            do {
                if (e->handle == id) {
                    s = e->song_addr;
                    if (s == NULL) {
                        s = e->fx_addr;
                    }
                    return s->num_waves;
                }
                e++;
            } while (++i < max_channels);
        }
    }
    return 0;
}
