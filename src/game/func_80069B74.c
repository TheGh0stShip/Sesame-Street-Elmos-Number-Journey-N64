#include "common.h"


/* rounds via 80068120 and adds 1.0 when x >= 0 and result != x (ceil-like) */
f64 ceil(f64 x) {
    f64 r = trunc(x);
    if ((x >= 0.0) & (r != x)) {
        return r + 1.0;
    }
    return r;
}
