#include "common.h"

typedef struct { u32 w0, w1; } Gfx;
typedef struct { u8 pad[0x34]; S34 *unk_34; } S73B4;
extern S73B4 *D_80077BC4;
Gfx *n_alEnvmixerPull(void *item, s32 arg, Gfx *g);
/* emits a display-list header then each item of the current list via n_alEnvmixerPull */
Gfx *func_80053750(s32 arg0, Gfx *g) {
    S34 *s = D_80077BC4->unk_34;
    void **items = s->unk_1C;
    s32 i;

    { Gfx *_g = g++; _g->w0 = 0x020007C0; _g->w1 = 0x2E0; }
    for (i = 0; i < s->unk_14; i++) {
        g = n_alEnvmixerPull(items[i], arg0, g);
    }
    return g;
}
