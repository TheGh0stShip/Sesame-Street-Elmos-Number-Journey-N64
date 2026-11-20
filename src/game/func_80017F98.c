/* keeps its own unprototyped gu*F declarations: called with s32 arguments where the SDK prototype takes float (a prototype converts them: code changes) */
#define GAME_PRIVATE_SDK
#include "common.h"

s32 guMtxF2L();
s32 guScaleF();

void func_80017F98(s32 arg0) {
    s32 sp18[16];
    s32 sp58[16];
    s32 sp98[16];
    s32 spD8;

    (*(f32 *)(arg0 + 84)) = *(f32 *)(arg0 + 48);
    ((struct gd_4_f32 *)(arg0 + 84))->v = ((struct gd_4_f32 *)(arg0 + 48))->v;
    ((struct gd_8_f32 *)(arg0 + 84))->v = ((struct gd_8_f32 *)(arg0 + 48))->v;
    ((struct gd_3C_s32 *)arg0)->v = 0;
    ((struct gd_40_f32 *)arg0)->v = 1.0f;
    ((struct gd_44_s32 *)arg0)->v = 0;
    ((struct gd_48_f32 *)arg0)->v = (((struct gd_34_f32 *)arg0)->v * ((struct gd_44_f32 *)arg0)->v) - (((struct gd_38_f32 *)arg0)->v * ((struct gd_40_f32 *)arg0)->v);
    ((struct gd_4C_f32 *)arg0)->v = (((struct gd_38_f32 *)arg0)->v * ((struct gd_3C_f32 *)arg0)->v) - (((struct gd_30_f32 *)arg0)->v * ((struct gd_44_f32 *)arg0)->v);
    ((struct gd_50_f32 *)arg0)->v = (((struct gd_30_f32 *)arg0)->v * ((struct gd_40_f32 *)arg0)->v) - (((struct gd_34_f32 *)arg0)->v * ((struct gd_3C_f32 *)arg0)->v);
    func_8001E74C(((s32)&sp18), (arg0 + 36), (arg0 + 84), (arg0 + 60), (arg0 + 72));
    guScaleF(((s32)&sp58), ((struct gd_60_s32 *)arg0)->v, ((struct gd_64_s32 *)arg0)->v, ((struct gd_68_s32 *)arg0)->v);
    guMtxCatF(((s32)&sp58), ((s32)&sp18), ((s32)&sp98));
    guMtxF2L(((s32)&sp98), (((struct gd_70_s32 *)arg0)->v + (D_800717E0 * 64)));
    spD8 = (s32)D_8013EC7C++;
    (*(s32 *)spD8) = 0x1020040;
    ((struct gd_4_s32 *)spD8)->v = ((struct gd_70_s32 *)arg0)->v + (D_800717E0 * 64);
}
