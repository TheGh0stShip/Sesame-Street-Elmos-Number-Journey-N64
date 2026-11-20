#include "common.h"

s32 func_80017A40(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp18;
    s32 pad0;
    s8 pad1;

    if ((arg0 < 0) || (arg0 >= 101)) {
        return 0;
    }
    sp18 = func_8004E620(arg0, arg1, arg2, 0, -1);
    return sp18;
}
