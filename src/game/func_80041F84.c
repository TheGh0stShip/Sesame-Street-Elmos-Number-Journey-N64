#include "common.h"


void func_80041F84(s32 arg0) {
    s32 sp10;
    s32 sp14;

    sp10 = *(s32 *)arg0;
    while (!(sp10 == 0)) {
        sp14 = *(s32 *)sp10;
        if ((((struct gd_C_s32 *)sp10)->v & 1) != 0) {
            func_80041E10(sp10, arg0);
        }
        sp10 = sp14;
    }
    return;
}
