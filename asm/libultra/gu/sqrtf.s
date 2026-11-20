/* func_80056690: libultra src/gu/func_80056690.s */
#include "asm.h"

.text
LEAF(func_80056690)
    sqrt.s  $f0, $f12
    j       $ra
END(func_80056690)

/* known names (symbols.number.tsv) */
#undef sqrtf
.globl sqrtf
sqrtf = func_80056690
