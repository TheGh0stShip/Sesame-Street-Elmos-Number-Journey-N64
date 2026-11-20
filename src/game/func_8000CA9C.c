#include "common.h"

s32 func_8000CA9C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f64 arg4) {
    if ((arg0 == 0) || (arg4 <= 0.0)) {
        return 0;
    }
    (*(f32 *)(arg0 + 4)) = *(f32 *)arg2;
    ((struct gd_4_f32 *)(arg0 + 4))->v = ((struct gd_4_f32 *)arg2)->v;
    ((struct gd_8_f32 *)(arg0 + 4))->v = ((struct gd_8_f32 *)arg2)->v;
    (*(f32 *)(arg0 + 16)) = *(f32 *)arg3;
    ((struct gd_4_f32 *)(arg0 + 16))->v = ((struct gd_4_f32 *)arg3)->v;
    ((struct gd_8_f32 *)(arg0 + 16))->v = ((struct gd_8_f32 *)arg3)->v;
    ((struct gd_20_f64 *)arg0)->v = D_80079FC0 / (arg4 * D_80079FB8);
    (*(s32 *)arg0) = arg1;
    return 1;
}
