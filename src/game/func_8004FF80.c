#include "common.h"
#include "libmus/player.h"

/* computes and stores unk_68 from remaining count (unk_AA - unk_B6) */
f32 func_8004FF80(channel_t *o) {
    s32 d = o->ticks - o->vib_delay;
    if (d > 0) {
        return o->vibrato = __sinf(d * o->vib_precalc) * o->vib_amount;
    }
    return 0.0f;
}
