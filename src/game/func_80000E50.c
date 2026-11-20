#include "common.h"

s32 func_80000E50(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp0;
    s32 pad0;
    s32 sp8;
    s32 spC;

    spC = 1;
    if (arg3 != 0) {
        sp8 = *(s32 *)arg3;
    }
    sp0 = 0;
    if (arg3 != 0) {
        if (arg4 != 0) {
            if (sp8 != (*(s32 *)arg3)) {
                (*(s32 *)arg4) = 0;
            }
        }
    }
    return spC;
}
