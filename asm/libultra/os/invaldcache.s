/* hand-written assembly routine (SDK) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_800583D0
func_800583D0:
    blez $5, .L800583D0_84
    nop
    addiu $11, $0, 0x2000
    sltu $1, $5, $11
    beqz $1, .L800583D0_8C
    nop
    addu $8, $4, $0
    addu $9, $4, $5
    sltu $1, $8, $9
    beqz $1, .L800583D0_84
    nop
    addiu $9, $9, -0x10
    andi $10, $8, 0xF
    beqz $10, .L800583D0_54
    nop
    subu $8, $8, $10
    cache 0x15, 0x0($8)
    sltu $1, $8, $9
    beqz $1, .L800583D0_84
    nop
    addiu $8, $8, 0x10
.L800583D0_54:
    andi $10, $9, 0xF
    beqz $10, .L800583D0_74
    nop
    subu $9, $9, $10
    cache 0x15, 0x10($9)
    sltu $1, $9, $8
    bnez $1, .L800583D0_84
    nop
.L800583D0_74:
    cache 0x11, 0x0($8)
    sltu $1, $8, $9
    bnez $1, .L800583D0_74
    addiu $8, $8, 0x10
.L800583D0_84:
    jr $31
    nop
.L800583D0_8C:
    lui $8, 0x8000
    addu $9, $8, $11
    addiu $9, $9, -0x10
.L800583D0_98:
    cache 0x01, 0x0($8)
    sltu $1, $8, $9
    bnez $1, .L800583D0_98
    addiu $8, $8, 0x10
    jr $31
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef osInvalDCache
.globl osInvalDCache
osInvalDCache = func_800583D0
