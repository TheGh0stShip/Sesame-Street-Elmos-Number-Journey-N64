#include "common.h"

/* returns the largest first word over a NULL-terminated table of pointers */
extern s32 *D_800778D4[];

s32 func_80051F28(void) {
    s32 **p = D_800778D4;
    s32 max = 0;
    s32 i;

    if (*p != NULL) {
        i = 0;
        do {
            if (max < *p[i]) {
                max = *p[i];
            }
            i++;
        } while (p[i] != NULL);
    }
    return max;
}
