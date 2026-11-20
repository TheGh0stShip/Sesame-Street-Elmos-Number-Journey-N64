#include "common.h"

f64 func_8000C9F8(s32 arg0, f64 arg2) {
    f64 sp0;

    if ((arg0 == 0) || (arg2 <= 0.0)) {
        return 0.0;
    }
    sp0 = ((struct gd_18_f64 *)arg0)->v;
    ((struct gd_18_f64 *)arg0)->v = D_80079FB0 / (arg2 * D_80079FA8);
    return sp0;
}
