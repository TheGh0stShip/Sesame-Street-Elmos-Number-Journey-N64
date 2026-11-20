#include "common.h"

void func_8004DE00(s32 arg0, s32 arg1) {
    s32 sp0;
    s32 sp4;

    sp4 = arg0;
    sp0 = 0;
    while (!(sp0 >= arg1)) {
        (*(s8 *)(sp4++)) = 0;
        sp0++;
    }
}
