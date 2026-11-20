/* hand-written assembly routine (SDK) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_80062D90
func_80062D90:
    blez $5, .L80062D90_48
    nop
    addiu $11, $0, 0x2000
    sltu $1, $5, $11
    beqz $1, .L80062D90_50
    nop
    addu $8, $4, $0
    addu $9, $4, $5
    sltu $1, $8, $9
    beqz $1, .L80062D90_48
    nop
    addiu $9, $9, -0x10
    andi $10, $8, 0xF
    subu $8, $8, $10
.L80062D90_38:
    cache 0x19, 0x0($8)
    sltu $1, $8, $9
    bnez $1, .L80062D90_38
    addiu $8, $8, 0x10
.L80062D90_48:
    jr $31
    nop
.L80062D90_50:
    lui $8, 0x8000
    addu $9, $8, $11
    addiu $9, $9, -0x10
.L80062D90_5C:
    cache 0x01, 0x0($8)
    sltu $1, $8, $9
    bnez $1, .L80062D90_5C
    addiu $8, $8, 0x10
    jr $31
    nop
    nop
    nop
    nop
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef osWritebackDCache
.globl osWritebackDCache
osWritebackDCache = func_80062D90
