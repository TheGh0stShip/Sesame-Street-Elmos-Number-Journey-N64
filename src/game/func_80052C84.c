#include "common.h"
#include "codec/stream.h"

extern stream_t *D_800A07C0;
/* allocates a free voice, starts it and returns its handle (0xFFFF if none) */
u16 func_80052C84(s32 arg0, s8 arg1, u16 arg2) {
    s32 i;
    stream_t *p;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].id == 0xFFFF) {
            p = &D_800A07C0[i];
            p->id = D_800A07C6 & 0xFFF; D_800A07C6++;
            func_8006ADB0(p, arg0, p->pcm, 0xA00, D_800A07CC);
            func_80052E44(D_800A07C0[i].id, arg1, arg2);
            D_800A07C0[i].frames_per_update = 4;
            return D_800A07C0[i].id;
        }
    }
    return 0xFFFF;
}
