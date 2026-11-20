/* __osGetCause: libultra 2.0F src/os/getcause.s. mfc0 of CP0 Cause: not expressible in C. */
#define C0_CAUSE $13
#define __osGetCause func_80068340
#include "asm.h"

.text
.text
LEAF(__osGetCause)
    MFC0($v0, C0_CAUSE)
    jr      $ra
END(__osGetCause)

/* known names (symbols.number.tsv) */
#undef __osGetCause
.globl __osGetCause
__osGetCause = func_80068340
