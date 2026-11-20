#include "common.h"

f64 func_8000CF84(s32 arg0, s32 arg1) {
    f64 sp10;
    f64 sp18;
    f64 sp20;

    sp20 = ((struct gd_20_f64 *)arg0)->v / D_80079FF0;
    sp10 = ((struct gd_8_f64 *)arg0)->v - ((struct gd_10_f64 *)arg0)->v;
    sp10 = func_80069B18(sp10, ((struct gd_20_f64 *)arg0)->v);
    if (sp10 > sp20) {
        sp10 = sp10 - ((struct gd_20_f64 *)arg0)->v;
    } else {
        if ((-sp20) >= sp10) {
            sp10 = sp10 + ((struct gd_20_f64 *)arg0)->v;
        }
    }
    sp18 = (((f64)(arg1 - (*(s32 *)arg0))) * sp10) * ((struct gd_18_f64 *)arg0)->v;
    ((struct gd_10_f64 *)arg0)->v = ((struct gd_10_f64 *)arg0)->v + sp18;
    (*(s32 *)arg0) = arg1;
    ((struct gd_10_f64 *)arg0)->v = func_80069B18(((struct gd_10_f64 *)arg0)->v, ((struct gd_20_f64 *)arg0)->v);
    if (((struct gd_10_f64 *)arg0)->v > sp20) {
        ((struct gd_10_f64 *)arg0)->v = ((struct gd_10_f64 *)arg0)->v - ((struct gd_20_f64 *)arg0)->v;
    } else {
        if (((struct gd_10_f64 *)arg0)->v <= (-sp20)) {
            ((struct gd_10_f64 *)arg0)->v = ((struct gd_10_f64 *)arg0)->v + ((struct gd_20_f64 *)arg0)->v;
        }
    }
    return ((struct gd_10_f64 *)arg0)->v;
}
