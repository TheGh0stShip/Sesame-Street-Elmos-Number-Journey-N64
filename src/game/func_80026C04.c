#include "common.h"


s32 func_80026C04(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((*(f32 *)arg1) < ((f32)((struct gd_6_s16 *)arg3)->v)) {
        if ((*(f32 *)arg1) > ((f32)((struct gd_4_s16 *)arg3)->v)) {
            if (((struct gd_4_f32 *)arg1)->v < ((f32)((struct gd_A_s16 *)arg3)->v)) {
                if (((struct gd_4_f32 *)arg1)->v > ((f32)((struct gd_8_s16 *)arg3)->v)) {
                    if (((struct gd_8_f32 *)arg1)->v < ((f32)((struct gd_E_s16 *)arg3)->v)) {
                        if (((struct gd_8_f32 *)arg1)->v > ((f32)((struct gd_C_s16 *)arg3)->v)) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    func_80026494(arg0, arg2, arg3);
    return 0;
}
