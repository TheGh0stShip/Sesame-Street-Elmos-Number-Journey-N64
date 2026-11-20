#include "common.h"

/* arcsine: range check then atan2(x, sqrt(1-x*x)) */
f64 asin(f64 x) {
    if ((x > 1.0) | (x < -1.0)) {
        func_8006A4E0(1, D_8007BDC0, x, 0.0, 0.0);
        return;
    }
    return atan2(x, sqrt(1.0 - x * x));
}
