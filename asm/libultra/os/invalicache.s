/* hand-written assembly routine (SDK) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_80062BE0
func_80062BE0:
    blez $5, .L80062BE0_48
    nop
    addiu $11, $0, 0x4000
    sltu $1, $5, $11
    beqz $1, .L80062BE0_50
    nop
    addu $8, $4, $0
    addu $9, $4, $5
    sltu $1, $8, $9
    beqz $1, .L80062BE0_48
    nop
    addiu $9, $9, -0x20
    andi $10, $8, 0x1F
    subu $8, $8, $10
.L80062BE0_38:
    cache 0x10, 0x0($8)
    sltu $1, $8, $9
    bnez $1, .L80062BE0_38
    addiu $8, $8, 0x20
.L80062BE0_48:
    jr $31
    nop
.L80062BE0_50:
    lui $8, 0x8000
    addu $9, $8, $11
    addiu $9, $9, -0x20
.L80062BE0_5C:
    cache 0x00, 0x0($8)
    sltu $1, $8, $9
    bnez $1, .L80062BE0_5C
    addiu $8, $8, 0x20
    jr $31
    nop
    nop
    nop
    nop
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef osInvalICache
.globl osInvalICache
osInvalICache = func_80062BE0
