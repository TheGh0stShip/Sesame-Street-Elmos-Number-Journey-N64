#include "common.h"

s32 func_8000D21C(s32 arg0, s32 arg1, u16 arg2, u16 arg3, f64 arg4) {
    if ((arg0 == 0) || (arg4 <= 0.0)) {
        return 0;
    }
    ((struct gd_4_s16 *)arg0)->v = arg2;
    ((struct gd_6_s16 *)arg0)->v = arg3;
    (*(s32 *)arg0) = arg1;
    ((struct gd_8_f64 *)arg0)->v = D_8007A008 / (arg4 * D_8007A000);
    return 1;
}
