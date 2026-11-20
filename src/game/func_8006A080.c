#include "common.h"

/* cosine-like: reduce by multiples of pi, take second result of sincos helper, negate for odd n */
f64 func_8006A080(f64 x) {
    s32 n;
    f64 s;
    f64 c;
    n = x / 3.141592653589793 + (x >= 0.0 ? 0.5 : -0.5);
    func_80069D80(x - n * 3.141592653589793, &s, &c);
    return (n & 1) ? -c : c;
}
