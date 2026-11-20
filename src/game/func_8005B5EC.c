#include "common.h"

/* libultra audio envelope volume step: ivol * rate^(samples/8) */
f32 func_8005B5EC(f32 ivol, s32 samples, s16 ratem, u16 ratel) {
    f32 r, a;
    s32 i;

    samples >>= 3;
    if (samples == 0) {
        return ivol;
    }
    r = ((f32)(ratem << 16) + (f32)ratel) / 65536;
    a = 1.0;
    for (i = 0; i < 32; i++) {
        if (samples & 1) {
            a *= r;
        }
        samples >>= 1;
        if (samples == 0) {
            break;
        }
        r *= r;
    }
    ivol *= a;
    return ivol;
}
