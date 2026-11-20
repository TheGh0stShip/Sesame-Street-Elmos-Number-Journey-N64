#include "common.h"


void func_8004CA34(s32 arg0) {
    register s32 reg_s0;
    register s32 reg_s1;
    register s32 reg_s2;

    reg_s0 = arg0;
    reg_s1 = ((struct gd_4_s32 *)reg_s0)->v;
    while (!(reg_s1 == 0)) {
        reg_s2 = ((struct gd_30_s32 *)reg_s1)->v;
        func_80042300(reg_s1);
        reg_s1 = reg_s2;
    }
}
