#include "common.h"

/* acos */
f64 acos(f64 x) {
    if (x > 1.0 || x < -1.0) {
        return func_8006A4E0(1, D_8007BDE0, x, 0.0, 0.0);
    }
    return 1.5707963267948966 - asin(x);
}
