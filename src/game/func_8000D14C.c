#include "common.h"

f64 func_8000D14C(s32 arg0, f64 arg2) {
    f64 sp10;
    f64 sp18;

    sp18 = ((struct gd_20_f64 *)arg0)->v / D_80079FF8;
    sp10 = func_80069B18(arg2, ((struct gd_20_f64 *)arg0)->v);
    if (sp10 > sp18) {
        sp10 = sp10 - ((struct gd_20_f64 *)arg0)->v;
    } else {
        if ((-sp18) >= sp10) {
            sp10 = sp10 + ((struct gd_20_f64 *)arg0)->v;
        }
    }
    ((struct gd_8_f64 *)arg0)->v = sp10;
    return sp10;
}
