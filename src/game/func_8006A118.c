#include "common.h"

/* tangent: reduce by pi, small values return x, else sin/cos ratio */
f64 tan(f64 x) {
    f64 s, c;
    f64 t;
    s32 n;
    t = x / 3.141592653589793;
    n = t + (x >= 0.0 ? 0.5 : -0.5);
    x -= n * 3.141592653589793;
    if ((x < 1e-08) & (x > -1e-08)) {
        return x;
    }
    func_80069D80(x, &s, &c);
    return s / c;
}
