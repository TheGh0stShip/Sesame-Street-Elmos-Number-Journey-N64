#include "common.h"

/* __osSpGetStatus: read SP_STATUS_REG */
u32 func_80068460(void) {
    return *(volatile u32 *)0xA4040010;
}
