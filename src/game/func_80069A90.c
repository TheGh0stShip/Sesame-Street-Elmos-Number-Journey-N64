#include "common.h"

/* truncates a double toward zero by masking off fraction bits */
f64 trunc(f64 x) {
    union { f64 d; s64 l; u32 w[2]; } u, r;
    s32 e;
    s64 m;
    s32 sh;
    u.d = x;
    e = u.w[0] >> 20;
    e &= 0x7FF;
    e -= 0x3FF;
    if (e < 0) {
        return 0.0;
    }
    sh = e + 11;
    m = (s64)0x8000000000000000LL >> sh;
    r.l = u.l & m;
    return r.d;
}
