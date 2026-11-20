#include "common.h"

void *func_800513E0(void *arg0, u8 *arg1) {
    (*(s8 *)((s8 *)(arg0) + (0xBD))) = (s8) ((u8) *arg1 >> 1);
    return arg1 + 1;
}
