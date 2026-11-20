#include "common.h"


s32 func_800217C4(s32 arg0) {
    s32 sp10;

    sp10 = func_80021640();
    if ((arg0 & 0xFFFF0000) != 0) {
        return (u32)sp10 % (u32)arg0;
        return;
    }
    return (u32)((u32)(sp10 & 0xFFFF0000) >> 16) % (u32)arg0;
}
