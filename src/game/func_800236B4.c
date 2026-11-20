#include "common.h"


f64 func_800236B4(s32 arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    f64 sp30;
    f64 sp38;
    f64 sp40;
    f64 sp48;

    sp10 = *(f32 *)(D_80076800 + 48);
    sp14 = ((struct gd_4_f32 *)(D_80076800 + 48))->v;
    sp18 = ((struct gd_8_f32 *)(D_80076800 + 48))->v;
    sp10 = sp10 - (*(f32 *)(D_80076800 + 36));
    sp14 = sp14 - ((struct gd_4_f32 *)(D_80076800 + 36))->v;
    sp18 = sp18 - ((struct gd_8_f32 *)(D_80076800 + 36))->v;
    if ((sp30 = (f64)sqrtf((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)))) >= D_8007A738) {
        sp48 = D_8007A740 / sp30;
        sp10 = (f32)(((f64)sp10) * sp48);
        sp14 = (f32)(((f64)sp14) * sp48);
        sp18 = (f32)(((f64)sp18) * sp48);
    } else {
    }
    sp20 = *(f32 *)arg0;
    sp24 = ((struct gd_4_f32 *)arg0)->v;
    sp28 = ((struct gd_8_f32 *)arg0)->v;
    sp20 = sp20 - (*(f32 *)(D_80076800 + 36));
    sp24 = sp24 - ((struct gd_4_f32 *)(D_80076800 + 36))->v;
    sp28 = sp28 - ((struct gd_8_f32 *)(D_80076800 + 36))->v;
    if ((sp30 = (f64)sqrtf((((sp20 * sp20) + (sp24 * sp24)) + (sp28 * sp28)))) >= D_8007A748) {
        sp48 = D_8007A750 / sp30;
        sp20 = (f32)(((f64)sp20) * sp48);
        sp24 = (f32)(((f64)sp24) * sp48);
        sp28 = (f32)(((f64)sp28) * sp48);
    } else {
    }
    sp38 = (f64)(((sp10 * sp20) + (sp14 * sp24)) + (sp18 * sp28));
    sp40 = sp30 * sp38;
    return sp40;
}
