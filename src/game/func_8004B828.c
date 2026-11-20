#include "common.h"

/* signed angular difference a - b, both wrapped into [0, 2pi] */
f32 func_8004B828(f32 a, f32 b)
{
    f32 opp;
    f32 d;

    while (a < 0) {
        a += 6.283185308;
    }
    while (a > 6.283185308) {
        a -= 6.283185308;
    }
    while (b < 0) {
        b += 6.283185308;
    }
    while (b > 6.283185308) {
        b -= 6.283185308;
    }
    opp = a + 3.141592654;
    if (opp > 6.283185308) {
        opp -= 6.283185308;
    }
    if (opp > a) {
        if (b >= a && b <= opp) {
            d = a - b;
        } else if (b < a) {
            d = a - b;
        } else {
            d = a + (6.283185308 - b);
        }
    } else {
        if (b >= a) {
            d = a - b;
        } else if (b > opp) {
            d = a - b;
        } else {
            d = -((6.283185308 - a) + b);
        }
    }
    return d;
}
