#include "common.h"


void func_80033A6C(s32 arg0) {
    s32 sp10;

    sp10 = arg0;
    if ((*(s32 *)sp10) == 1) {
        goto L13;
    }
    func_80059540(((s32)&D_8007A960), ((s32)&D_8007A964), 3679);
L13: ;
    D_80107CE0 = func_80042080(88);
    func_80069C70(D_80107CE0, 0, 88);
    D_80076800 = D_80107CE0;
    ((struct gd_8_s16 *)D_80107CE0)->v = ((struct gd_2_u16 *)sp10)->v;
    ((struct gd_A_s16 *)D_80107CE0)->v = ((struct gd_6_u16 *)sp10)->v;
    ((struct gd_C_s32 *)D_80107CE0)->v = ((struct gd_8_s32 *)sp10)->v;
    ((struct gd_14_s32 *)D_80107CE0)->v = (s32)&func_80023408;
    ((struct gd_18_s32 *)D_80107CE0)->v = 0;
    ((struct gd_1C_s32 *)D_80107CE0)->v = 0;
    ((struct gd_20_s32 *)D_80107CE0)->v = 0;
    ((struct gd_10_s32 *)D_80107CE0)->v = (s32)&func_80033528;
    ((struct gd_24_f32 *)D_80107CE0)->v = ((struct gd_C_f32 *)sp10)->v;
    ((struct gd_28_f32 *)D_80107CE0)->v = ((struct gd_10_f32 *)sp10)->v;
    ((struct gd_2C_f32 *)D_80107CE0)->v = ((struct gd_14_f32 *)sp10)->v;
    ((struct gd_24_s32 *)D_80107CE0)->v = 0;
    ((struct gd_28_f32 *)D_80107CE0)->v = 5e+02f;
    ((struct gd_2C_s32 *)D_80107CE0)->v = 0;
    ((struct gd_30_s32 *)D_80107CE0)->v = 0;
    ((struct gd_34_s32 *)D_80107CE0)->v = 0;
    ((struct gd_38_s32 *)D_80107CE0)->v = 0;
    ((struct gd_3C_s32 *)D_80107CE0)->v = 0;
    ((struct gd_40_f32 *)D_80107CE0)->v = 1.0f;
    ((struct gd_44_s32 *)D_80107CE0)->v = 0;
    func_80041A80(D_80107CE0, D_80115408, 3, 0);
}
