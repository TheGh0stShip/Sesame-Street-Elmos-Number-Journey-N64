#include "common.h"
                             /* extern */

f64 fmod(f64 arg0, f64 arg1) {
    f64 var_f0_10;

    var_f0_10 = 0.0;
    if (arg1 != 0.0) {
        var_f0_10 = arg0 - (trunc(arg0 / arg1) * arg1);
    }
    return var_f0_10;
}
