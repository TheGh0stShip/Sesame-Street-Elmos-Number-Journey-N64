#include "common.h"


void MusPtrBankSetCurrent(void *arg0) {
    if ((arg0 != NULL) && ((*(s32 *)((s8 *)(arg0) + (0x10))) < 0)) {
        D_800A056C = arg0;
    }
}
