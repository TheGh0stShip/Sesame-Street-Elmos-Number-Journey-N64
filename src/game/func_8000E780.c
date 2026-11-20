#include "common.h"

s32 func_8000E780(s32 arg0) {
    s32 sp0;

    if (D_80072EDC == 0) {
        return -1;
    }
    if ((arg0 == D_80072ECC) || (arg0 == D_80072ED0)) {
        return 0;
    }
    if ((arg0 < 0) || (arg0 > D_80072EC0)) {
        arg0 = 0;
    }
    sp0 = D_80072ECC;
    D_80072ED0 = D_80072ED8 = arg0;
    D_80072ECC = D_80072ED4 = arg0;
    return sp0;
}
