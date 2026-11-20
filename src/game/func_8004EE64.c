#include "common.h"

s32 MusFxBankGetPtrBank(void *arg0) {
    return (*(s32 *)((s8 *)(arg0) + (0x10)));
}
