#include "common.h"


void *MusPtrBankSetSingle(void *arg0) {
    if ((arg0 != NULL) && ((*(s32 *)((s8 *)(arg0) + (0x10))) < 0)) {
        D_800A0568 = arg0;
    }
    return D_800A0568;
}
