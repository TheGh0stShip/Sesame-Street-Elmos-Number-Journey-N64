/* range check (a0 > 7 exits) then calls into the following routine with the return address kept in at */
/* hand-written assembly routine (SDK) */
/* hand-written assembly: clamps a1 into [0x28,0x78] window before storing to 0x166(a0), then tail-branches */
/* audio/sample filter loop: shifts a 0x78-halfword buffer and accumulates a scaled history (table offset from the caller via at) */
/* hand-written assembly routine (SDK) */
/* hand-written assembly routine (SDK) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_8006C700
func_8006C700:
    addiu $1, $4, -7
    bgtz $1, . + 0x38
    nop
    addu $1, $31, $0
    bltzal $0, . + 0xDC
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006C714
func_8006C714:
    nop
    addiu $15, $31, 0xD4
    addu $31, $1, $0
    addu $15, $15, $4
    lb $1, 0x0($15)
    nop
    sra $8, $1, 4
    andi $9, $1, 0x7
    beq $0, $0, .L8006C714_34
    nop
    addiu $8, $4, -0x8
    sra $8, $8, 3
    andi $9, $4, 0x7
.L8006C714_34:
    sll $12, $9, 11
    addiu $12, $12, 0x47FF
    ori $10, $0, 0x6
    subu $10, $10, $8
    ori $11, $0, 0x1
    addiu $1, $10, -0x1
    sllv $11, $11, $1
    ori $14, $0, 0xD
    addu $24, $6, $0
.L8006C714_58:
    lh $13, 0x0($24)
    nop
    sll $13, $13, 13
    addiu $13, $13, -0x7000
    mult $13, $12
    mflo $13
    addiu $13, $13, 0x4000
    sra $13, $13, 15
    addu $13, $13, $11
    srav $13, $13, $10
    sh $13, 0x0($24)
    addiu $14, $14, -0x1
    bgtz $14, .L8006C714_58
    addiu $24, $24, 0x2
    addu $25, $7, $0
    ori $14, $0, 0x28
.L8006C714_98:
    sh $0, 0x0($25)
    addiu $14, $14, -0x1
    bgtz $14, .L8006C714_98
    addiu $25, $25, 0x2
    addu $25, $7, $5
    addu $25, $25, $5
    addu $24, $6, $0
    ori $14, $0, 0xD
.L8006C714_B8:
    lh $1, 0x0($24)
    addiu $24, $24, 0x2
    sh $1, 0x0($25)
    addiu $14, $14, -0x1
    bgtz $14, .L8006C714_B8
    addiu $25, $25, 0x6
    jr $31
    nop
    lwc1 $f23, -0x1C19($30)
    scd $19, -0xA09($15)
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006C7F4
func_8006C7F4:
lh $11, 0x166($4)
addiu $1, $5, -0x28
bltz $1, 1f
nop
addiu $1, $1, -0x50
bgtz $1, 1f
nop
addu $11, $5, $0
1: sh $11, 0x166($4)
addu $1, $31, $0
.word 0x04100022 
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006C820
func_8006C820:
    nop
    addiu $8, $31, 0x84
    addu $31, $1, $0
    addu $8, $8, $6
    addu $8, $8, $6
    lhu $8, 0x0($8)
    ori $10, $0, 0x77
    addiu $13, $4, 0x0
.L8006AEB0_20:
    lhu $12, 0x50($13)
    addiu $10, $10, -0x1
    sh $12, 0x0($13)
    bgez $10, .L8006AEB0_20
    addiu $13, $13, 0x2
    ori $10, $0, 0x27
    addiu $14, $4, 0x0
    addiu $13, $4, 0x0
    subu $13, $13, $11
    subu $13, $13, $11
    addu $15, $7, $0
.L8006AEB0_4C:
    lh $9, 0xF0($13)
    addiu $13, $13, 0x2
    mult $9, $8
    mflo $9
    addiu $9, $9, 0x4000
    sra $9, $9, 15
    lh $1, 0x0($15)
    addiu $15, $15, 0x2
    addu $9, $9, $1
    sh $9, 0xF0($14)
    addiu $14, $14, 0x2
    bgtz $10, .L8006AEB0_4C
    addiu $10, $10, -0x1
    jr $31
    nop
.set at
.set reorder

.set noreorder
.set noat
func_8006C8A8:   /* QLB table of the long-term synthesis filter, read ra-relative */
    .half 3277, 11469, 21299, 32767
.globl func_8006C8B0
func_8006C8B0:
.L8006C8A8_8:
    lh $10, 0x0($9)
    addiu $9, $9, 0x2
    sll $13, $25, 16
    sra $13, $13, 16
    mult $23, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    sra $13, $25, 16
    mult $22, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    mult $13, $10
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $23, $1, $22
    sll $13, $24, 16
    sra $13, $13, 16
    mult $21, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    mult $13, $10
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $22, $1, $21
    sra $13, $24, 16
    mult $20, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    mult $13, $10
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $21, $1, $20
    sll $13, $15, 16
    sra $13, $13, 16
    mult $19, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    mult $13, $10
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $20, $1, $19
    sra $13, $15, 16
    mult $18, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    mult $13, $10
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $19, $1, $18
    sll $13, $14, 16
    sra $13, $13, 16
    mult $17, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    mult $13, $10
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $18, $1, $17
    sra $13, $14, 16
    mult $16, $13
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    subu $10, $10, $1
    mult $13, $10
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $17, $1, $16
    addu $16, $10, $0
    ori $1, $0, 0x6E14
    mult $1, $7
    mflo $1
    addiu $1, $1, 0x4000
    sra $1, $1, 15
    addu $10, $10, $1
    addu $7, $10, $0
    sll $10, $10, 1
    sra $1, $10, 15
    addiu $1, $1, 0x1
    sra $1, $1, 1
    beqz $1, .L8006C8A8_1B0
    nop
    bltz $1, .L8006C8A8_1B0
    addiu $10, $0, -0x8000
    addiu $10, $0, 0x7FFF
.L8006C8A8_1B0:
    andi $10, $10, 0xFFF8
    sh $10, 0x0($8)
    addiu $8, $8, 0x2
    bgtz $11, .L8006C8A8_8
    addiu $11, $11, -0x1
    jr $31
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006CA74
func_8006CA74:
    addiu $29, $29, -0x44
    sw $31, 0x0($29)
    sw $4, 0x4($29)
    sw $5, 0x8($29)
    sw $6, 0xC($29)
    sw $7, 0x10($29)
    sw $16, 0x14($29)
    sw $17, 0x18($29)
    sw $18, 0x1C($29)
    sw $19, 0x20($29)
    sw $20, 0x24($29)
    sw $21, 0x28($29)
    sw $22, 0x2C($29)
    sw $23, 0x30($29)
    addiu $1, $4, 0x140
    lw $16, 0x0($1)
    lw $17, 0x4($1)
    lw $18, 0x8($1)
    lw $19, 0xC($1)
    lw $20, 0x10($1)
    lw $21, 0x14($1)
    lw $22, 0x18($1)
    lw $23, 0x1C($1)
    lh $7, 0x164($4)
    bltzal $0, .L8006CA74_1F8
    nop
    addiu $11, $31, 0x190
    bal .L8006CA74_154
    nop
    addiu $9, $5, 0x0
    addiu $8, $6, 0x0
    bal . - 0x240
    ori $11, $0, 0xC
    bltzal $0, .L8006CA74_210
    nop
    addiu $11, $31, 0x184
    bal .L8006CA74_154
    nop
    addiu $9, $5, 0x1A
    addiu $8, $6, 0x1A
    bal . - 0x264
    ori $11, $0, 0xD
    bltzal $0, .L8006CA74_220
    nop
    addiu $11, $31, 0x170
    bal .L8006CA74_154
    nop
    addiu $9, $5, 0x36
    addiu $8, $6, 0x36
    bal . - 0x288
    ori $11, $0, 0xC
    bltzal $0, .L8006CA74_238
    nop
    addiu $11, $31, 0x164
    bal .L8006CA74_154
    nop
    addiu $9, $5, 0x50
    addiu $8, $6, 0x50
    bal . - 0x2AC
    ori $11, $0, 0x77
    addiu $1, $4, 0x140
    sw $16, 0x0($1)
    sw $17, 0x4($1)
    sw $18, 0x8($1)
    sw $19, 0xC($1)
    sw $20, 0x10($1)
    sw $21, 0x14($1)
    sw $22, 0x18($1)
    sw $23, 0x1C($1)
    sh $7, 0x164($4)
    lw $31, 0x0($29)
    lw $4, 0x4($29)
    lw $5, 0x8($29)
    lw $6, 0xC($29)
    lw $7, 0x10($29)
    lw $16, 0x14($29)
    lw $17, 0x18($29)
    lw $18, 0x1C($29)
    lw $19, 0x20($29)
    lw $20, 0x24($29)
    lw $21, 0x28($29)
    lw $22, 0x2C($29)
    lw $23, 0x30($29)
    jr $31
    addiu $29, $29, 0x44
.L8006CA74_154:
    lh $1, 0x188($4)
    addiu $8, $4, 0x168
    beqz $1, .L8006CA74_16C
    addiu $9, $8, 0x10
    addu $8, $9, $0
    addiu $9, $4, 0x168
.L8006CA74_16C:
    addu $13, $31, $0
    lh $10, 0x0($8)
    jalr $11
    lh $12, 0x0($9)
    sll $14, $10, 16
    lh $10, 0x2($8)
    jalr $11
    lh $12, 0x2($9)
    or $14, $14, $10
    lh $10, 0x4($8)
    jalr $11
    lh $12, 0x4($9)
    sll $15, $10, 16
    lh $10, 0x6($8)
    jalr $11
    lh $12, 0x6($9)
    or $15, $15, $10
    lh $10, 0x8($8)
    jalr $11
    lh $12, 0x8($9)
    sll $24, $10, 16
    lh $10, 0xA($8)
    jalr $11
    lh $12, 0xA($9)
    or $24, $24, $10
    lh $10, 0xC($8)
    jalr $11
    lh $12, 0xC($9)
    sll $25, $10, 16
    lh $10, 0xE($8)
    jalr $11
    lh $12, 0xE($9)
    addu $31, $13, $0
    jr $31
    or $25, $25, $10
.L8006CA74_1F8:
    sra $10, $10, 2
    sra $12, $12, 1
    addu $10, $10, $12
    sra $12, $12, 1
    beq $0, $0, .L8006CA74_238
    addu $10, $10, $12
.L8006CA74_210:
    sra $10, $10, 1
    sra $12, $12, 1
    beq $0, $0, .L8006CA74_238
    addu $10, $10, $12
.L8006CA74_220:
    sra $10, $10, 1
    sra $12, $12, 2
    addu $12, $12, $10
    sra $10, $10, 1
    beq $0, $0, .L8006CA74_238
    addu $10, $10, $12
.L8006CA74_238:
    bgez $10, .L8006CA74_248
    addu $12, $0, $0
    negu $10, $10
    ori $12, $0, 0x1
.L8006CA74_248:
    addiu $1, $10, -0x2B33
    bltz $1, .L8006CA74_268
    sll $1, $10, 1
    addiu $1, $10, -0x4E66
    bltz $1, .L8006CA74_268
    addiu $1, $10, 0x2B33
    sra $1, $10, 2
    addiu $1, $1, 0x6600
.L8006CA74_268:
    addiu $10, $1, -0x7FFF
    blez $10, .L8006CA74_278
    addu $10, $1, $0
    ori $10, $0, 0x7FFF
.L8006CA74_278:
    bnez $12, .L8006CA74_284
    negu $10, $10
    negu $10, $10
.L8006CA74_284:
    jr $31
    andi $10, $10, 0xFFFF
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef Gsm_Long_Term_Synthesis_Filtering
.globl Gsm_Long_Term_Synthesis_Filtering
Gsm_Long_Term_Synthesis_Filtering = func_8006C820
