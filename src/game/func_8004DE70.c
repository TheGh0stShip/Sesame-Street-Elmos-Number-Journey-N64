#include "common.h"

void func_8004DE70(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp0;
    s32 sp4;
    s32 sp8;

    sp4 = arg0;
    sp8 = arg1;
    sp0 = 0;
    while (!(sp0 >= arg2)) {
        (*(s8 *)(sp8++)) = *(u8 *)(sp4++);
        sp0++;
    }
}
