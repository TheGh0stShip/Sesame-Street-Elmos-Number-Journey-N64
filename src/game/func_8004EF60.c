#include "common.h"

void *__MusIntMemMalloc(s32);
/* allocates a table of n (clamped 64..1024) 8-byte entries */
void func_8004EF60(s32 n) {
    if (n < 0x40) {
        n = 0x40;
    } else if (n > 0x400) {
        n = 0x400;
    }
    D_800A0590 = __MusIntMemMalloc(n * 8);
    D_800A058C = n;
    D_800A0588 = 0;
    D_800A0584 = 0;
}
