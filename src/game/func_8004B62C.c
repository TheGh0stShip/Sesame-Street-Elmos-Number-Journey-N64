#include "common.h"


void func_8004B62C(s32 arg0) {
    register s32 reg_s0;
    register s32 reg_s1;

    reg_s0 = ((struct gd_C_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        reg_s1 = ((struct gd_28_s32 *)reg_s0)->v;
        func_80042300(reg_s0);
        reg_s0 = reg_s1;
    }
    ((struct gd_C_s32 *)arg0)->v = 0;
}
