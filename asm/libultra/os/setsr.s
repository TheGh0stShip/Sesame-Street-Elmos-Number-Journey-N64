/* __osSetSR: libultra 2.0F src/os/setsr.s. mtc0 to CP0 Status plus hazard nop: not expressible in C. */
#define C0_SR $12
#define __osSetSR func_80062D80
#include "asm.h"

.text
.text
LEAF(__osSetSR)
    MTC0($a0, C0_SR)
    NOP
    jr      $ra
END(__osSetSR)

/* known names (symbols.number.tsv) */
#undef __osSetSR
.globl __osSetSR
__osSetSR = func_80062D80
