/* hand-written: returns 1 when the SP status register's halt/broke bits are clear (SP busy test) */
/* hand-written: wait for SP DMA idle, then write SP_STATUS 0x82 and wait for halt */
/* writes 0xC5 to PI register 0xA4040010 via the shared tail of the previous routine */
/* hand-written: write 2 to MI 0xA430000C (interrupt mask) and 0x125 to SP_STATUS (0xA4040010) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_800685A0
func_800685A0:
    addu $2, $0, $0
    lui $8, 0xA404
    lw $8, 0x10($8)
    andi $8, $8, 0x3
    bnez $8, 1f
    nop
    ori $2, $2, 0x1
1:  jr $31
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_800685C4
func_800685C4:
lui $a0, 0xA404
 ori $a0, $a0, 0x18
1: lw $v0, 0($a0)
 bnez $v0, 1b
 nop
addiu $a1, $zero, 0x82
 lui $a0, 0xA404
 ori $a0, $a0, 0x10
 sw $a1, 0($a0)
2: lui $a0, 0xA404
 ori $a0, $a0, 0x10
 lw $v0, 0($a0)
 andi $v0, $v0, 0x3
 beqz $v0, 2b
 nop
jr $ra
 nop
.set reorder
.set at

.set noreorder
.set noat
.globl func_80068608
func_80068608:
    lui $4, 0xA404
    ori $4, $4, 0x10
    addiu $5, $0, 0xC5
    beq $0, $0, . - 0x2C
    sw $5, 0($4)
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006861C
func_80066CAC:
lui $a0, 0xA430
ori $a0, $a0, 0xC
addiu $a1, $zero, 0x2
sw $a1, 0x0($a0)
lui $a0, 0xA404
ori $a0, $a0, 0x10
addiu $a1, $zero, 0x125
jr $ra
sw $a1, 0x0($a0)
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef __rmonRCPrunning
.globl __rmonRCPrunning
__rmonRCPrunning = func_800685A0
#undef __rmonIdleRCP
.globl __rmonIdleRCP
__rmonIdleRCP = func_800685C4
#undef __rmonStepRCP
.globl __rmonStepRCP
__rmonStepRCP = func_80068608
