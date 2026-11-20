#include "common.h"

s32 func_8004B5C8(s32 arg0, s32 arg1) {
    register s32 reg_v1;

    reg_v1 = ((struct gd_C_s32 *)arg0)->v;
    while (!(reg_v1 == 0)) {
        if ((*(s32 *)reg_v1) == arg1) {
            return reg_v1;
        }
        reg_v1 = ((struct gd_28_s32 *)reg_v1)->v;
    }
    return 0;
}
