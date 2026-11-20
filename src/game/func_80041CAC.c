#include "common.h"

s32 func_80041CAC(s32 arg0, u16 arg1, u16 arg2) {
    s32 sp4;

    sp4 = arg0;
    if (arg1 == 0xFFFF) {
        if (arg2 == 0xFFFF) {
            return sp4;
        }
        for (; !(sp4 == 0); sp4 = *(s32 *)sp4) {
            if (((struct gd_A_u16 *)sp4)->v == arg2) {
                return sp4;
            }
        }
        return 0;
    }
    if (arg2 == 0xFFFF) {
        while (!(sp4 == 0)) {
            if (((struct gd_8_u16 *)sp4)->v == arg1) {
                return sp4;
            }
            sp4 = *(s32 *)sp4;
        }
        return 0;
    }
    for (; !(sp4 == 0); sp4 = *(s32 *)sp4) {
        if (((struct gd_8_u16 *)sp4)->v == arg1) {
            if (((struct gd_A_u16 *)sp4)->v == arg2) {
                return sp4;
            }
        }
    }
    return 0;
}
