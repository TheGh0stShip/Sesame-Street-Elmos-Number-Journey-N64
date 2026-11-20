/* ROM entry: clear BSS, set the stack, jump to the main entry function */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_80000400
func_80000400:
    lui $8, %hi(D_8007BF20)
    addiu $8, $8, %lo(D_8007BF20)
    lui $9, 0xC
    addiu $9, $9, 0x5040
1:  sw $0, 0x0($8)
    sw $0, 0x4($8)
    addi $8, $8, 0x8
    addi $9, $9, -0x8
    bnez $9, 1b
    nop
    lui $10, %hi(func_8000C040)
    addiu $10, $10, %lo(func_8000C040)
    lui $29, %hi(D_800B1090)
    jr $10
    addiu $29, $29, %lo(D_800B1090)
.set at
.set reorder
