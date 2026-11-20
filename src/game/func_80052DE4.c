#include "common.h"
#include "codec/stream.h"

extern stream_t *D_800A07C0;
/* is an entity with id arg0 present */
s32 func_80052DE4(u16 arg0) {
    s32 i;
    if (arg0 == 0xFFFF) return 0;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].id == arg0) return 1;
    }
    return 0;
}
