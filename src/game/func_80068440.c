#include "common.h"

/* __osSpDeviceBusy */
int func_80068440(void) {
    register u32 stat = *(volatile u32 *)0xA4040010;
    if (stat & (0x4 | 0x8 | 0x10)) return 1; else return 0;
}
