#include "common.h"

extern s32 func_8003325C;

void func_80033368(s32 arg0) {
    s32 sp10;
    s32 sp14;

    sp10 = arg0;
    if ((*(s32 *)sp10) == 19) {
        goto L13;
    }
    func_80059540(((s32)&D_8007A960), ((s32)&D_8007A964), 3486);
L13: ;
    sp14 = func_80042080(48);
    func_80069C70(sp14, 0, 48);
    ((struct gd_8_s16 *)sp14)->v = ((struct gd_2_u16 *)sp10)->v;
    ((struct gd_A_s16 *)sp14)->v = ((struct gd_6_u16 *)sp10)->v;
    ((struct gd_C_s32 *)sp14)->v = ((struct gd_8_s32 *)sp10)->v;
    ((struct gd_14_s32 *)sp14)->v = (s32)&func_80033224;
    ((struct gd_18_s32 *)sp14)->v = 0;
    ((struct gd_1C_s32 *)sp14)->v = 0;
    ((struct gd_20_s32 *)sp14)->v = 0;
    ((struct gd_10_s32 *)sp14)->v = (s32)&func_8003325C;
    ((struct gd_24_f32 *)sp14)->v = ((struct gd_C_f32 *)sp10)->v;
    ((struct gd_28_f32 *)sp14)->v = ((struct gd_10_f32 *)sp10)->v;
    ((struct gd_2C_f32 *)sp14)->v = ((struct gd_14_f32 *)sp10)->v;
    func_80041A80(sp14, D_80115408, 3, 0);
}
