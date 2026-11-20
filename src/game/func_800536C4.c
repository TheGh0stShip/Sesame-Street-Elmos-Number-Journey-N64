#include "common.h"


s32 func_800536C4(u8 *p) {
    if (D_800A07DC + 0xB8 < p) {
        if (D_80077910 != 0) { D_80077910 = 0; return D_800A07D4; }
    } else if (p < D_800A07DC) {
        if (D_80077910 != 0) { D_80077910 = 0; return D_800A07D8; }
    } else {
        D_80077910 = 1;
    }
    return D_800A07D0;
}
