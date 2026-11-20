#include "common.h"


/* atan2(y, x) on top of atan, with a domain-error report for (0, 0) */
f64 atan2(f64 y, f64 x) {
    f64 r;

    if ((x == 0.0) & (y == 0.0)) {
        return func_8006A4E0(1, D_8007BE28, x, y, 0.0);
    }
    if (x == 0.0) {
        if (y > 0.0) {
            return 1.5707963267948966;
        }
        return -1.5707963267948966;
    }
    r = atan(y / x);
    if (x > 0.0) {
        return r;
    }
    if (y == 0.0) {
        return 3.141592653589793;
    }
    if (y > 0.0) {
        return r + 3.141592653589793;
    }
    return r - 3.141592653589793;
}
