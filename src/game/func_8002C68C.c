#include "common.h"

extern s32 D_80106C84;

s32 func_8002C68C(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    f32 sp0;
    f32 sp4;

    sp0 = (((struct gd_24_f32 *)arg0)->v - arg1) - ((struct gd_24_f32 *)D_80106C84)->v;
    sp4 = (((struct gd_2C_f32 *)arg0)->v - arg2) - ((struct gd_2C_f32 *)D_80106C84)->v;
    sp0 = sp0 * sp0;
    sp4 = sp4 * sp4;
    sp4 = sp4 + sp0;
    if (sp4 < arg3) {
        return 1;
    }
    return 0;
}
