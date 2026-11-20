#include "common.h"

/* CORDIC sine and cosine in 2^60 fixed point */
void func_80069D80(f64 a, f64 *s, f64 *c) {
    s64 x = 0x09B74EDA8435E580LL;
    s64 y = 0;
    s64 z;
    s64 nx;
    s32 i;
    z = (s64)(a * 1152921504606846976.0);
    for (i = 0; i < 52; i++) {
        if (z >= 0) {
            nx = x - (y >> i);
            y += x >> i;
            z -= D_80079010[i];
        } else {
            nx = x + (y >> i);
            y -= x >> i;
            z += D_80079010[i];
        }
        x = nx;
    }
    *s = (f64)y / 1152921504606846976.0;
    *c = (f64)x / 1152921504606846976.0;
}
