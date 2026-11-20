#include "common.h"


void func_80046590(s32 arg0, s32 arg1) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    s32 pad0;
    f32 sp30;
    f32 sp34;
    f32 sp38;

    sp30 = (*(f32 *)(arg0 + 16)) - (*(f32 *)(arg1 + 16));
    sp34 = ((struct gd_4_f32 *)(arg0 + 16))->v - ((struct gd_4_f32 *)(arg1 + 16))->v;
    sp38 = ((struct gd_8_f32 *)(arg0 + 16))->v - ((struct gd_8_f32 *)(arg1 + 16))->v;
    sp20 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
    sp10 = (*(f32 *)arg0) + ((struct gd_1C_f32 *)arg1)->v;
    if (sp20 < sp10) {
        ((struct gd_34_s32 *)arg0)->v = *(s32 *)arg1;
        ((struct gd_3C_s32 *)arg0)->v = arg1;
        if ((((struct gd_2C_s32 *)arg0)->v & 1) != 0) {
            ((struct gd_38_s32 *)arg0)->v = 0;
            return;
        }
        sp30 = (*(f32 *)(arg0 + 4)) - (*(f32 *)(arg1 + 16));
        sp34 = ((struct gd_4_f32 *)(arg0 + 4))->v - ((struct gd_4_f32 *)(arg1 + 16))->v;
        sp38 = ((struct gd_8_f32 *)(arg0 + 4))->v - ((struct gd_8_f32 *)(arg1 + 16))->v;
        sp24 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
        sp18 = sp24 - sp10;
        if (sp18 <= 0.0f) {
            sp18 = 0;
        }
        sp1C = sp10 - sp20;
        sp14 = sp18 + sp1C;
        if (sp14 <= 0.001f) {
            sp28 = 0;
        } else {
            sp28 = sp18 / sp14;
        }
        sp28 = sp28 * ((struct gd_28_f32 *)arg0)->v;
        ((struct gd_38_f32 *)arg0)->v = sp28;
    }
}
