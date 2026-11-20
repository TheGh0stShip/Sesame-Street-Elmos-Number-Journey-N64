#include "common.h"

/* floor(): truncate, step down for negative non-integers */
f64 floor(f64 x) {
    f64 t = trunc(x);
    if ((x < 0.0) & (t != x)) {
        return t - 1.0;
    }
    return t;
}
