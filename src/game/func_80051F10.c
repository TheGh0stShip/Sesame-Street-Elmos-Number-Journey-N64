#include "common.h"

void func_80051F10(void *arg0) {
    (*(s8 *)((s8 *)(arg0) + (0x1C))) = 6;
    (*(s32 **)((s8 *)(arg0) + (0x20))) = &D_80077748;
}
