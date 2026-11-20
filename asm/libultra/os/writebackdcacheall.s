/* hand-written assembly routine (SDK) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_80058580
func_80058580:
    lui $8, 0x8000
    addiu $10, $0, 0x2000
    addu $9, $8, $10
    addiu $9, $9, -0x10
.L80058580_10:
    cache 0x01, 0x0($8)
    sltu $1, $8, $9
    bnez $1, .L80058580_10
    addiu $8, $8, 0x10
    jr $31
    nop
    nop
    nop
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef osWritebackDCacheAll
.globl osWritebackDCacheAll
osWritebackDCacheAll = func_80058580
