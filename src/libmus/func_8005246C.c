#include "common.h"

/* extern */
/* extern */
/* extern */

extern s32 D_800A05E4;

void __OsSchedInstall(void) {
    s32 temp_v0_7;

    temp_v0_7 = __MusIntMemMalloc(0x58);
    D_800A05E4 = temp_v0_7;
    osCreateMesgQueue(temp_v0_7 + 8, temp_v0_7 + 0x20, 4);
    osCreateMesgQueue(D_800A05E4 + 0x30, D_800A05E4 + 0x48, 4);
    osScAddClient(D_800A05E0, D_800A05E4, D_800A05E4 + 8);
}
