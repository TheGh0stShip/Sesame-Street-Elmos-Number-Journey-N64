/* func_80062D70: libultra src/os/setfpccsr.s */
#include "asm.h"

.text
LEAF(func_80062D70)
    CFC1(   $v0, $31)
    CTC1(   $a0, $31)
    j       $ra
END(func_80062D70)

/* known names (symbols.number.tsv) */
#undef __osSetFpcCsr
.globl __osSetFpcCsr
__osSetFpcCsr = func_80062D70
