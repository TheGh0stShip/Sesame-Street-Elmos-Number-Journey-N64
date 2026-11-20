#include "common.h"

/* __osAiDeviceBusy: returns 1 if the AI FIFO is full */
s32 func_80063730(void) {
    s32 status = *(volatile u32 *)0xA450000C;
    if (status & 0x80000000) {
        return 1;
    } else {
        return 0;
    }
}
