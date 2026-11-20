#include "common.h"

void func_8004B354(s32 arg0, s32 arg1) {
    register s32 reg_s0;

    if (arg0 == 0) {
        return;
    }
    reg_s0 = ((struct gd_8_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        if ((*(s32 *)reg_s0) == arg1) {
            if (((struct gd_48_s32 *)reg_s0)->v != 0) {
                ((s32 (*)())((struct gd_48_s32 *)reg_s0)->v)(((struct gd_14_s32 *)reg_s0)->v);
            }
        }
        reg_s0 = ((struct gd_18_s32 *)reg_s0)->v;
    }
}
