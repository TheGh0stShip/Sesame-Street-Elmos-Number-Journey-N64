#include "common.h"


void func_8004A318(s32 arg0, s32 arg1) {
    s32 sp28[16];
    s32 sp68[16];
    u16 spA8;
    s32 spAC;

    ((s32 (*)(s32, s32, s32, f32, f32, f32, f32))guPerspectiveF)(((s32)&sp68), ((s32)&spA8), D_80108FB4, ((f32)(D_8007B410 / ((f64)D_800B1092))), 5e+01f, ((struct gd_F0_f32 *)arg0)->v, 1.0f);
    if (arg1 != 0) {
        spAC = (s32)D_8013EC7C++;
        (*(s32 *)spAC) = 0xBC00000E;
        ((struct gd_4_s32 *)spAC)->v = spA8;
    }
    ((s32 (*)(s32, s32, s32, s32, f32, f32, f32, f32, f32, f32))guLookAtF)(((s32)&sp28), ((struct gd_60_s32 *)arg0)->v, ((struct gd_64_s32 *)arg0)->v, ((struct gd_68_s32 *)arg0)->v, ((struct gd_10_f32 *)arg0)->v, ((struct gd_14_f32 *)arg0)->v, ((struct gd_18_f32 *)arg0)->v, ((struct gd_84_f32 *)arg0)->v, ((struct gd_88_f32 *)arg0)->v, ((struct gd_8C_f32 *)arg0)->v);
    guMtxCatF(((s32)&sp28), ((s32)&sp68), ((s32)&D_8013ED00));
}
