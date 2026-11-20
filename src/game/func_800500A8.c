#include "common.h"
#include "libmus/player.h"


/* advances a byte-coded event stream: value bytes set unk_70/unk_24, durations reload unk_A4 */
void func_800500A8(channel_t *t) {
    u8 c;
    f32 v;
    s32 hi;

    do {
        (*(s32 *)&t->pitchbend_frame) += 0x100;
        if (--t->pitchbend_count == 0) {
            c = *t->ppitchbend++;
            if (c >= 0x80) {
                v = (f32)(c & 0x7F) - 64.0;
                t->pitchbend = v;
                t->pitchbend_precalc = v * t->bendrange;
                c = *t->ppitchbend++;
                if (c >= 0x80) {
                    hi = (c & 0x7F) << 8;
                    t->pitchbend_count = hi;
                    t->pitchbend_count = *t->ppitchbend++ + (hi + 2);
                } else {
                    t->pitchbend_count = c + 2;
                }
            } else {
                v = (f32)c - 64.0;
                t->pitchbend_count = 1;
                t->pitchbend = v;
                t->pitchbend_precalc = v * t->bendrange;
            }
        }
    } while ((*(s32 *)&t->pitchbend_frame) - (*(s32 *)&t->channel_frame) < 0);
}
