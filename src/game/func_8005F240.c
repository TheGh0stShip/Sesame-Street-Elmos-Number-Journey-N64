#include "common.h"

/* osAiGetStatus: read AI_STATUS_REG */
u32 func_8005F240(void) { return *(volatile u32 *)0xA450000C; }
