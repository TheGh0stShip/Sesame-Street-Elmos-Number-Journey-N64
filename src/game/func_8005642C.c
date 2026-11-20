#include "common.h"

/* extern */
                              /* extern */
extern void *D_80077BC4;

void func_8005642C(void) {
    s32 var_s0_10;

    var_s0_10 = (*(s32 *)((s8 *)(D_80077BC4) + (0x14)));
    if (var_s0_10 != 0) {
        do {
            alUnlink(var_s0_10);
            alLink(var_s0_10, D_80077BC4 + 4);
            var_s0_10 = (*(s32 *)((s8 *)(D_80077BC4) + (0x14)));
        } while (var_s0_10 != 0);
    }
}
