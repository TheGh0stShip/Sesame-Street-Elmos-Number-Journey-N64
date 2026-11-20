#include "common.h"


void func_8004B3E4(s32 arg0) {
    register s32 reg_s0;
    register s32 reg_s1;

    if (arg0 == 0) {
        return;
    }
    reg_s0 = ((struct gd_8_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        reg_s1 = ((struct gd_18_s32 *)reg_s0)->v;
        if (((struct gd_48_s32 *)reg_s0)->v != 0) {
            ((s32 (*)())((struct gd_48_s32 *)reg_s0)->v)(((struct gd_14_s32 *)reg_s0)->v);
        }
        func_80042300(reg_s0);
        reg_s0 = reg_s1;
    }
    ((struct gd_8_s32 *)arg0)->v = 0;
}
