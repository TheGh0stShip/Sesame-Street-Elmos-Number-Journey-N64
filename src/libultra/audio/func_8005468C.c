#include <libaudio.h>
#include "synthInternals.h"
#include <os.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


s32 _getVol(s32 arg0, s32 arg1, s16 arg2, s32 arg3) {
    s32 temp_a1_5;
    s32 var_v0_19;

    temp_a1_5 = arg1 >> 3;
    if (temp_a1_5 != 0) {
        var_v0_19 = (arg0 + (((s32) ((arg3 & 0xFFFF) * temp_a1_5) >> 0x10) + (arg2 * temp_a1_5))) << 0x10;
    } else {
        var_v0_19 = arg0 << 0x10;
    }
    return var_v0_19 >> 0x10;
}
