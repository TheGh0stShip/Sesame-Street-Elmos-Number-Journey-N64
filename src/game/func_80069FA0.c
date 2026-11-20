#include "common.h"


/* reduce x by multiples of pi, then evaluate via func_80069D80; sign flips on odd multiples */
f64 func_80069FA0(f64 x) {
    f64 r;
    f64 tmp;
    s32 n;
    n = (s32)(x / 3.141592653589793 + (x >= 0.0 ? 0.5 : -0.5));
    x = x - n * 3.141592653589793;
    if ((x < 1e-08) & (x > -1e-08)) {
        return x;
    }
    func_80069D80(x, &r, &tmp);
    return (n & 1) ? -r : r;
}
