#include "common.h"

void func_80041F04(s32 arg0) {
    s32 sp10;

    sp10 = *(s32 *)arg0;
    while (!(sp10 == 0)) {
        if (((struct gd_10_s32 *)sp10)->v != 0) {
            ((s32 (*)())((struct gd_10_s32 *)sp10)->v)(sp10);
        }
        sp10 = *(s32 *)sp10;
    }
    return;
}
