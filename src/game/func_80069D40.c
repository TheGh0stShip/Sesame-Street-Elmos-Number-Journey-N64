#include "common.h"

s32 rand(void) {
    s32 temp_lo_9;

    temp_lo_9 = D_800A7A40 * 0x41C64E6D;
    D_800A7A40 = temp_lo_9 + 0x3039;
    return ((s32) (temp_lo_9 + 0x303A) >> 0x10) & 0x7FFF;
}
