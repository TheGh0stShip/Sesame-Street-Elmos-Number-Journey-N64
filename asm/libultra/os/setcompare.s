/* __osSetCompare: libultra 2.0F src/os/setcompare.s. mtc0 to CP0 Compare: not expressible in C. */
#define C0_COMPARE $11
#define __osSetCompare func_80062D60
#include "asm.h"

.text
.text
LEAF(__osSetCompare)
    MTC0($a0, C0_COMPARE)
    jr      $ra
END(__osSetCompare)

/* known names (symbols.number.tsv) */
#undef __osSetCompare
.globl __osSetCompare
__osSetCompare = func_80062D60
