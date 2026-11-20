#include "common.h"


void func_800497D0(s32 arg0, s32 arg1, register s32 arg2, register s32 arg3, s32 arg4, s32 arg5) {
    u16 sp10;
    u16 sp12;
    s32 sp14;

    sp10 = arg2;
    sp12 = arg3;
    sp14 = *(s32 *)arg1;
    for (;;) {
        if (sp14 == 0) {
            break;
        }
        sp14 = func_80041CAC(sp14, sp10, sp12);
        if (sp14 != 0) {
            func_800496B8(arg0, sp14, arg4, arg5);
            sp14 = *(s32 *)sp14;
        }
    }
    return;
}
