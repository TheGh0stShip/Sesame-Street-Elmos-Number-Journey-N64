#include "common.h"


void func_80033850(s32 arg0) {
    register s32 reg_s0;
    s32 sp28[16];
    s32 sp68[16];
    s32 spA8[22];

    reg_s0 = arg0;
    if (D_800AD03A != 0) {
        ((s32 (*)(s32, s32, s32, s32, f32, f32, f32))guPerspectiveF)(((s32)&sp28), ((s32)&D_80109DE0), 0x42200000, 0x3FAAAAAB, 5e+01f, 1e+04f, 1.0f);
        ((s32 (*)(s32, s32, s32, s32, f32, f32, f32, f32, f32, f32))guLookAtF)(((s32)&sp68), ((struct gd_24_s32 *)reg_s0)->v, ((struct gd_28_s32 *)reg_s0)->v, ((struct gd_2C_s32 *)reg_s0)->v, ((struct gd_30_f32 *)reg_s0)->v, ((struct gd_34_f32 *)reg_s0)->v, ((struct gd_38_f32 *)reg_s0)->v, ((struct gd_3C_f32 *)reg_s0)->v, ((struct gd_40_f32 *)reg_s0)->v, ((struct gd_44_f32 *)reg_s0)->v);
        guMtxCatF(((s32)&sp68), ((s32)&sp28), ((s32)&spA8));
        guMtxF2L(((s32)&spA8), D_8013EF28);
        addActorDL(((s32)&D_80033498), reg_s0, 1, 0, 0);
        return;
    }
    if (((struct gd_54_u8 *)reg_s0)->v != 0) {
        ((struct gd_54_s8 *)reg_s0)->v--;
        ((struct gd_30_f32 *)reg_s0)->v = ((struct gd_30_f32 *)reg_s0)->v + ((struct gd_48_f32 *)reg_s0)->v;
        ((struct gd_34_f32 *)reg_s0)->v = ((struct gd_34_f32 *)reg_s0)->v + ((struct gd_4C_f32 *)reg_s0)->v;
        ((struct gd_38_f32 *)reg_s0)->v = ((struct gd_38_f32 *)reg_s0)->v + ((struct gd_50_f32 *)reg_s0)->v;
        ((struct gd_24_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_24_f32 *)reg_s0)->v) + D_8007AE10);
        ((struct gd_28_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_28_f32 *)reg_s0)->v) + D_8007AE18);
        ((struct gd_2C_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_2C_f32 *)reg_s0)->v) + D_8007AE20);
        D_8006F75C = (f32)(((f64)D_8006F75C) + D_8007AE28);
    }
    if (((struct gd_54_u8 *)reg_s0)->v == 0) {
        if (D_800769F0 == 4) {
            D_800769F0 = 5;
        }
    }
}
