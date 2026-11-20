#include "common.h"


void func_80037FD0(s32 arg0) {
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        if (((struct gd_1D4_s32 *)arg0)->v == 10) {
            func_80037450(arg0, 11);
            return;
        }
        func_800375A8(arg0, 0);
    }
}
