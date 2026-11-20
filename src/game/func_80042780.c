#include "common.h"


f32 func_80042780(s32 arg0, s32 arg1) {
    f32 sp10;
    f32 sp14;

    sp10 = (((*(f32 *)arg0) * (*(f32 *)arg0)) + (((struct gd_4_f32 *)arg0)->v * ((struct gd_4_f32 *)arg0)->v)) + (((struct gd_8_f32 *)arg0)->v * ((struct gd_8_f32 *)arg0)->v);
    sp14 = sqrtf(sp10);
    if (sp14 == 0.0f) {
        return 0.0f;
    }
    (*(f32 *)arg1) = (*(f32 *)arg0) / sp14;
    ((struct gd_4_f32 *)arg1)->v = ((struct gd_4_f32 *)arg0)->v / sp14;
    ((struct gd_8_f32 *)arg1)->v = ((struct gd_8_f32 *)arg0)->v / sp14;
    return sp14;
}
