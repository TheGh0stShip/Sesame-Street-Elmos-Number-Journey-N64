#include "common.h"

s32 func_80012E78(void) {
    register s32 reg_v1;

    reg_v1 = D_80128548;
    if (reg_v1 != 0) {
        D_80128548 = *(s32 *)reg_v1;
        if (D_800A7F74 != 0) {
            ((struct gd_4_s32 *)D_800A7F74)->v = reg_v1;
        }
        ((struct gd_4_s32 *)reg_v1)->v = 0;
        (*(s32 *)reg_v1) = D_800A7F74;
        D_800A7F74 = reg_v1;
        D_8013C72C++;
        return reg_v1;
        return;
    }
    return 0;
}
