#include "common.h"

                            /* extern */

s32 n_alResampleParam(void) {
    ((s32 (*)())n_alLoadParam)();
    return 0;
}
