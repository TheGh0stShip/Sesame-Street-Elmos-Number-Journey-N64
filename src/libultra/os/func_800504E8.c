#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
/* not in the SDK headers */
extern s32 D_800A0564;


/* random number in [0, range) from a shift-register seed */

s32 func_800504E8(s32 range)
{
    s32 i;
    s32 t;
    for (i = 0; i < 8; i++) {
        t = D_800A0564 & 0x48000000;
        D_800A0564 = D_800A0564 << 1;
        if (t == 0x48000000 || t == 0x08000000) {
            D_800A0564 |= 1;
        }
    }
    return range * ((f32)D_800A0564 / 65536.0f / 65536.0f);
}
