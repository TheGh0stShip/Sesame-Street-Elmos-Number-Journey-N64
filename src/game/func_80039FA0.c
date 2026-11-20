#include "common.h"


void func_80039FA0(s32 arg0) {
    func_80037450(arg0, 17);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v | 128;
    if (D_80109D4C == 0) {
        ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    }
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 4;
    D_800A7F2C = 0;
}
