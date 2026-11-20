#include "common.h"


void func_8001EFF8(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp10;
    s32 sp14;

    sp10 = 0;
    while (!((*(s32 *)((sp10 * 4) + arg2)) == 0)) {
        if (func_8001F0CC(arg0, arg1) != 0) {
            sp14 = (s32)D_8013EC7C++;
            (*(s32 *)sp14) = 0x6000000;
            ((struct gd_4_s32 *)sp14)->v = *(s32 *)((sp10 * 4) + arg2);
        }
        arg1 = arg1 + 24;
        sp10++;
    }
}
