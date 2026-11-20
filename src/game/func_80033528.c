#include "common.h"

extern s32 D_80106C84;

void func_80033528(s32 arg0) {
    register s32 reg_s0;
    s32 sp28[16];
    s32 sp68[16];
    s32 spA8[22];

    reg_s0 = arg0;
    if (D_800AD03A != 0) {
        ((s32 (*)(s32, s32, s32, f32, f32, f32, f32))guPerspectiveF)(((s32)&sp28), ((s32)&D_80109DE0), D_80108FB4, ((f32)(D_8007AE08 / ((f64)D_800B1092))), 5e+01f, 1e+04f, 1.0f);
        ((struct gd_30_f32 *)reg_s0)->v = ((struct gd_24_f32 *)D_80106C84)->v;
        ((struct gd_34_f32 *)reg_s0)->v = ((struct gd_28_f32 *)D_80106C84)->v;
        ((struct gd_38_f32 *)reg_s0)->v = ((struct gd_2C_f32 *)D_80106C84)->v;
        ((s32 (*)(s32, s32, s32, s32, f32, f32, f32, f32, f32, f32))guLookAtF)(((s32)&sp68), ((struct gd_24_s32 *)reg_s0)->v, ((struct gd_28_s32 *)reg_s0)->v, ((struct gd_2C_s32 *)reg_s0)->v, ((struct gd_30_f32 *)reg_s0)->v, ((struct gd_34_f32 *)reg_s0)->v, ((struct gd_38_f32 *)reg_s0)->v, ((struct gd_3C_f32 *)reg_s0)->v, ((struct gd_40_f32 *)reg_s0)->v, ((struct gd_44_f32 *)reg_s0)->v);
        guMtxCatF(((s32)&sp68), ((s32)&sp28), ((s32)&spA8));
        guMtxF2L(((s32)&spA8), D_8013EF28);
        addActorDL(((s32)&D_80033498), reg_s0, 1, 0, 0);
    } else {
    }
}
