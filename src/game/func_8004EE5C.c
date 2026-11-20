#include "common.h"

void MusFxBankSetPtrBank(void *arg0, s32 arg1) {
    (*(s32 *)((s8 *)(arg0) + (0x10))) = arg1;
}
