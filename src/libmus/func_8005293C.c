#include "common.h"
/* calls alHeapDBAlloc(0, 0, &D_800A07B0, 1, a) */

void alHeapDBAlloc(s32, s32, void *, s32, s32);
void __MusIntMemMalloc(s32 a) {
    alHeapDBAlloc(0, 0, (*(u8 (*)[])&D_800A07B0), 1, a);
}
