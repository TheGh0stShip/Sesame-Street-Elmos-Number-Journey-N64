#include "common.h"

s32 func_8004A050();

s32 func_80049970(s32 arg0, s32 arg1, f32 arg2, f32 arg3) {
    register s32 reg_s0;

    reg_s0 = func_80042080(256);
    if (reg_s0 == 0) {
        return 0;
    }
    func_80069C70(reg_s0, 0, 256);
    ((struct gd_4_s32 *)reg_s0)->v = 1;
    ((struct gd_F0_f32 *)reg_s0)->v = arg3;
    ((struct gd_FC_f32 *)reg_s0)->v = *(f32 *)arg1;
    ((struct gd_10_f32 *)reg_s0)->v = *(f32 *)arg0;
    ((struct gd_14_f32 *)reg_s0)->v = ((struct gd_4_f32 *)arg0)->v;
    ((struct gd_18_f32 *)reg_s0)->v = ((struct gd_8_f32 *)arg0)->v;
    ((struct gd_1C_f32 *)reg_s0)->v = *(f32 *)arg1;
    ((struct gd_20_f32 *)reg_s0)->v = ((struct gd_4_f32 *)arg1)->v;
    ((struct gd_24_f32 *)reg_s0)->v = ((struct gd_8_f32 *)arg1)->v;
    ((struct gd_28_f32 *)reg_s0)->v = arg2;
    ((struct gd_30_f32 *)reg_s0)->v = *(f32 *)arg1;
    ((struct gd_34_s32 *)reg_s0)->v = 0;
    if (((struct gd_1C_f32 *)reg_s0)->v < 0.0f) {
        ((struct gd_1C_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_1C_f32 *)reg_s0)->v) + D_8007B400);
    }
    memcpy((reg_s0 + 56), (reg_s0 + 16), 40);
    func_8004A050(reg_s0);
    func_8004A318(reg_s0, 1);
    func_8004A478(reg_s0);
    if (D_80076800 != 0) {
        func_80049D60(reg_s0);
    }
    return reg_s0;
}
