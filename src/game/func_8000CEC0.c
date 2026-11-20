#include "common.h"

s32 func_8000CEC0(s32 arg0, s32 arg1, f64 arg2, f64 arg4, f64 arg6, f64 arg8) {
    if ((arg0 == 0) || (arg6 <= 0.0)) {
        return 0;
    }
    ((struct gd_8_f64 *)arg0)->v = arg2;
    ((struct gd_10_f64 *)arg0)->v = arg4;
    ((struct gd_18_f64 *)arg0)->v = D_80079FE8 / (arg6 * D_80079FE0);
    (*(s32 *)arg0) = arg1;
    ((struct gd_20_f64 *)arg0)->v = arg8;
    return 1;
}
