#include "common.h"
#include "codec/stream.h"

void *__MusIntMemMalloc(s32);
extern stream_t *D_800A07C0;
/* allocate n voice blocks with buffers */
s32 func_80052A30(s32 n, s32 arg1, s32 arg2, void *cb) {
    s32 i;
    (*(s16 (*))&D_800A07C4) = 0;
    (*(s16 (*))&D_800A07C6) = 0;
    D_800A07C8 = arg1;
    D_800A07C0 = __MusIntMemMalloc(n * sizeof(stream_t));
    if (D_800A07C0 == NULL) return 0;
    for (i = 0; i < n; i++) {
        D_800A07C0[i].id = 0xFFFF;
        if ((D_800A07C0[i].pcm = __MusIntMemMalloc(0x1400)) == NULL) return 0;
        if ((D_800A07C0[i].rs_state = __MusIntMemMalloc(0x20)) == NULL) return 0;
    }
    (*(s16 (*))&D_800A07C4) = n;
    D_800A07CC = cb;
    if (cb == NULL) D_800A07CC = D_80053574;
    return 1;
}
