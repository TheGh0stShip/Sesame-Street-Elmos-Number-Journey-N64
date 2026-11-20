/* hand-written assembly routine (SDK) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_800675F0
func_800675F0:
    beqz $6, .L800675F0_70
    addu $7, $5, $0
    beq $4, $5, .L800675F0_70
    nop
    slt $1, $5, $4
    bnez $1, .L800675F0_34
    nop
    add $2, $4, $6
    slt $1, $5, $2
    beqz $1, .L800675F0_34
    nop
    beq $0, $0, .L800675F0_1A0
    nop
.L800675F0_34:
    slti $1, $6, 0x10
    bnez $1, .L800675F0_50
    nop
    andi $2, $4, 0x3
    andi $3, $5, 0x3
    beq $2, $3, .L800675F0_78
    nop
.L800675F0_50:
    beqz $6, .L800675F0_70
    nop
    addu $3, $4, $6
.L800675F0_5C:
    lb $2, 0x0($4)
    addiu $4, $4, 0x1
    sb $2, 0x0($5)
    bne $4, $3, .L800675F0_5C
    addiu $5, $5, 0x1
.L800675F0_70:
    jr $31
    addu $2, $7, $0
.L800675F0_78:
    beqz $2, .L800675F0_E4
    nop
    addiu $1, $0, 0x1
    beq $2, $1, .L800675F0_C8
    nop
    addiu $1, $0, 0x2
    beq $2, $1, .L800675F0_B0
    nop
    lb $2, 0x0($4)
    addiu $4, $4, 0x1
    sb $2, 0x0($5)
    addiu $5, $5, 0x1
    beq $0, $0, .L800675F0_E4
    addiu $6, $6, -0x1
.L800675F0_B0:
    lh $2, 0x0($4)
    addiu $4, $4, 0x2
    sh $2, 0x0($5)
    addiu $5, $5, 0x2
    beq $0, $0, .L800675F0_E4
    addiu $6, $6, -0x2
.L800675F0_C8:
    lb $2, 0x0($4)
    lh $3, 0x1($4)
    addiu $4, $4, 0x3
    sb $2, 0x0($5)
    sh $3, 0x1($5)
    addiu $5, $5, 0x3
    addiu $6, $6, -0x3
.L800675F0_E4:
    slti $1, $6, 0x20
    bnez $1, .L800675F0_140
    nop
    lw $2, 0x0($4)
    lw $3, 0x4($4)
    lw $8, 0x8($4)
    lw $9, 0xC($4)
    lw $10, 0x10($4)
    lw $11, 0x14($4)
    lw $12, 0x18($4)
    lw $13, 0x1C($4)
    addiu $4, $4, 0x20
    sw $2, 0x0($5)
    sw $3, 0x4($5)
    sw $8, 0x8($5)
    sw $9, 0xC($5)
    sw $10, 0x10($5)
    sw $11, 0x14($5)
    sw $12, 0x18($5)
    sw $13, 0x1C($5)
    addiu $5, $5, 0x20
    beq $0, $0, .L800675F0_E4
    addiu $6, $6, -0x20
.L800675F0_140:
    slti $1, $6, 0x10
    bnez $1, .L800675F0_17C
    nop
    lw $2, 0x0($4)
    lw $3, 0x4($4)
    lw $8, 0x8($4)
    lw $9, 0xC($4)
    addiu $4, $4, 0x10
    sw $2, 0x0($5)
    sw $3, 0x4($5)
    sw $8, 0x8($5)
    sw $9, 0xC($5)
    addiu $5, $5, 0x10
    beq $0, $0, .L800675F0_140
    addiu $6, $6, -0x10
.L800675F0_17C:
    slti $1, $6, 0x4
    bnez $1, .L800675F0_50
    nop
    lw $2, 0x0($4)
    addiu $4, $4, 0x4
    sw $2, 0x0($5)
    addiu $5, $5, 0x4
    beq $0, $0, .L800675F0_17C
    addiu $6, $6, -0x4
.L800675F0_1A0:
    add $4, $4, $6
    add $5, $5, $6
    slti $1, $6, 0x10
    bnez $1, .L800675F0_1C4
    nop
    andi $2, $4, 0x3
    andi $3, $5, 0x3
    beq $2, $3, .L800675F0_1F4
    nop
.L800675F0_1C4:
    beqz $6, .L800675F0_70
    nop
    addiu $4, $4, -0x1
    addiu $5, $5, -0x1
    subu $3, $4, $6
.L800675F0_1D8:
    lb $2, 0x0($4)
    addiu $4, $4, -0x1
    sb $2, 0x0($5)
    bne $4, $3, .L800675F0_1D8
    addiu $5, $5, -0x1
    jr $31
    addu $2, $7, $0
.L800675F0_1F4:
    beqz $2, .L800675F0_260
    nop
    addiu $1, $0, 0x3
    beq $2, $1, .L800675F0_244
    nop
    addiu $1, $0, 0x2
    beq $2, $1, .L800675F0_22C
    nop
    lb $2, -0x1($4)
    addiu $4, $4, -0x1
    sb $2, -0x1($5)
    addiu $5, $5, -0x1
    beq $0, $0, .L800675F0_260
    addiu $6, $6, -0x1
.L800675F0_22C:
    lh $2, -0x2($4)
    addiu $4, $4, -0x2
    sh $2, -0x2($5)
    addiu $5, $5, -0x2
    beq $0, $0, .L800675F0_260
    addiu $6, $6, -0x2
.L800675F0_244:
    lb $2, -0x1($4)
    lh $3, -0x3($4)
    addiu $4, $4, -0x3
    sb $2, -0x1($5)
    sh $3, -0x3($5)
    addiu $5, $5, -0x3
    addiu $6, $6, -0x3
.L800675F0_260:
    slti $1, $6, 0x20
    bnez $1, .L800675F0_2BC
    nop
    lw $2, -0x4($4)
    lw $3, -0x8($4)
    lw $8, -0xC($4)
    lw $9, -0x10($4)
    lw $10, -0x14($4)
    lw $11, -0x18($4)
    lw $12, -0x1C($4)
    lw $13, -0x20($4)
    addiu $4, $4, -0x20
    sw $2, -0x4($5)
    sw $3, -0x8($5)
    sw $8, -0xC($5)
    sw $9, -0x10($5)
    sw $10, -0x14($5)
    sw $11, -0x18($5)
    sw $12, -0x1C($5)
    sw $13, -0x20($5)
    addiu $5, $5, -0x20
    beq $0, $0, .L800675F0_260
    addiu $6, $6, -0x20
.L800675F0_2BC:
    slti $1, $6, 0x10
    bnez $1, .L800675F0_2F8
    nop
    lw $2, -0x4($4)
    lw $3, -0x8($4)
    lw $8, -0xC($4)
    lw $9, -0x10($4)
    addiu $4, $4, -0x10
    sw $2, -0x4($5)
    sw $3, -0x8($5)
    sw $8, -0xC($5)
    sw $9, -0x10($5)
    addiu $5, $5, -0x10
    beq $0, $0, .L800675F0_2BC
    addiu $6, $6, -0x10
.L800675F0_2F8:
    slti $1, $6, 0x4
    bnez $1, .L800675F0_1C4
    nop
    lw $2, -0x4($4)
    addiu $4, $4, -0x4
    sw $2, -0x4($5)
    addiu $5, $5, -0x4
    beq $0, $0, .L800675F0_2F8
    addiu $6, $6, -0x4
    nop
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef bcopy
.globl bcopy
bcopy = func_800675F0
