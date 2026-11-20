/* signed 64-bit compare of (a0:a1) with (a2:a3) returning a 0/1/2 code (branch targets as in the ROM) */
/* hand-written asm tail of a 64-bit unsigned compare: (a0:a1) vs (a2:a3); branches into the neighbouring
   less-than / greater-than exits at 0x80068C10 / 0x80068C18 and returns 1 when equal */
/* 64-bit multiply (a0:a1 * a2:a3), low 64 bits in v0:v1 */
/* unsigned 64-bit shift-subtract divide of (a0:a1) by (a2:a3): quotient in v1-pair, remainder left in a0:a1 */
/* 64-bit unsigned divide helper: 64-bit dividend (a0:a1) by 32-bit divisor a3, iterative quotient refinement */
/* signed 64-bit divide: negates negative operands, calls the unsigned routine (ra in t5), negates the result when signs differ */
/* signed 64-bit remainder/divide wrapper: negates negative operands, calls the unsigned routine with ra kept in t5, negates the result */
/* 64-bit unsigned divide/remainder dispatch: tail-calls the 32-bit-divisor path when the divisor high word is zero */
/* 64-bit unsigned divide wrapper: calls the 32-bit or 64-bit divisor routine keeping ra in t5, returns the quotient */
/* converts a double in f12 to a signed 64-bit integer (truncating), returned in v0:v1 */
/* hand-written assembly: convert signed 64-bit integer (a0:a1) to double */
/* float (f12) to 64-bit integer conversion, saturating to 0x80000000:00000000 on overflow */
/* hand-written asm: converts the signed 64-bit integer in (a0:a1) to a float */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_8006A550
func_8006A550:
    slt $25, $4, $6
    bnez $25, 1f
    nop
    beq $4, $6, 2f
    nop
    slt $25, $5, $7
    bnez $25, 1f
    nop
    beq $5, $7, 2f
    nop
    jr $31
    addiu $2, $0, 1
    1: jr $31
    xor $2, $2, $2
    2: jr $31
    addiu $2, $0, 2
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A590
func_8006A590:
    sltu $25, $4, $6
    bnez $25, .-20
    nop
    bne $6, $4, .-20
    nop
    sltu $25, $5, $7
    bnez $25, .-40
    nop
    bne $7, $5, .-40
    nop
    jr $31
    addiu $2, $0, 1
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A5C4
func_8006A5C4:
    multu $5, $6
    mflo $8
    nop
    nop
    multu $4, $7
    mflo $3
    addu $8, $8, $3
    nop
    multu $4, $6
    mflo $3
    mfhi $2
    jr $31
    addu $2, $2, $8
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A5F8
func_8006A5F8:
    xor $3, $3, $3
    sltu $25, $6, $4
    bnez $25, .L80068C88_48
    nop
    bne $4, $6, .L80068C88_40
    nop
    sltu $25, $7, $5
    bnez $25, .L80068C88_48
    nop
    bne $5, $7, .L80068C88_40
    nop
    xor $5, $5, $5
    xor $4, $4, $4
    addiu $3, $0, 0x1
    jr $31
    xor $2, $2, $2
.L80068C88_40:
    jr $31
    xor $2, $2, $2
.L80068C88_48:
    addiu $2, $0, 0x1
    lui $8, 0x8000
    and $25, $6, $8
    bnez $25, .L80068C88_A4
    nop
    srl $25, $7, 31
    sll $6, $6, 1
    or $6, $6, $25
    sll $7, $7, 1
    sltu $25, $6, $4
    bnez $25, .L80068C88_8C
    nop
    bne $4, $6, .L80068C88_94
    nop
    sltu $25, $7, $5
    beqz $25, .L80068C88_94
    nop
.L80068C88_8C:
    j func_8006A648
    sll $2, $2, 1
.L80068C88_94:
    sll $25, $6, 31
    srl $7, $7, 1
    or $7, $7, $25
    srl $6, $6, 1
.L80068C88_A4:
    sltu $25, $6, $4
    bnez $25, .L80068C88_C4
    nop
    bne $4, $6, .L80068C88_D8
    nop
    sltu $25, $5, $7
    bnez $25, .L80068C88_D8
    nop
.L80068C88_C4:
    sltu $25, $5, $7
    subu $5, $5, $7
    subu $4, $4, $6
    subu $4, $4, $25
    or $3, $3, $2
.L80068C88_D8:
    sll $25, $6, 31
    srl $7, $7, 1
    or $7, $7, $25
    srl $6, $6, 1
    srl $2, $2, 1
    bnez $2, .L80068C88_A4
    nop
    jr $31
    xor $2, $2, $2
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A6F4
func_8006A6F4:
    divu $0, $4, $7
    bnez $7, .L80068D84_10
    nop
    break 7
.L80068D84_10:
    mflo $10
    mfhi $4
    xor $8, $8, $8
    beqz $4, .L80068D84_98
    nop
    addu $3, $5, $0
    addu $2, $4, $0
    addu $9, $7, $0
.L80068D84_30:
    sll $25, $2, 31
    srl $3, $3, 1
    or $3, $3, $25
    srl $2, $2, 1
    bnez $2, .L80068D84_30
    srl $9, $9, 1
    addiu $9, $9, 0x1
    divu $0, $3, $9
    bnez $9, .L80068D84_5C
    nop
    break 7
.L80068D84_5C:
    mflo $3
    xor $2, $2, $2
    addu $8, $8, $3
    sltu $25, $8, $3
    addu $10, $10, $2
    addu $10, $10, $25
    nop
    multu $3, $7
    mflo $3
    mfhi $2
    sltu $25, $5, $3
    subu $5, $5, $3
    subu $4, $4, $2
    j func_8006A710
    subu $4, $4, $25
.L80068D84_98:
    divu $0, $5, $7
    bnez $7, .L80068D84_A8
    nop
    break 7
.L80068D84_A8:
    mflo $3
    mfhi $9
    xor $2, $2, $2
    addu $3, $3, $8
    sltu $25, $3, $8
    addu $2, $2, $10
    jr $31
    addu $2, $2, $25
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A7C0
func_8006A7C0:
    lui $8, 0x8000
    addu $3, $4, $0
    xor $3, $3, $6
    and $25, $8, $4
    beqz $25, .L80068E50_30
    nop
    addiu $9, $0, -0x1
    xor $5, $5, $9
    xor $4, $4, $9
    addiu $5, $5, 0x1
    sltiu $25, $5, 0x1
    addu $4, $4, $25
.L80068E50_30:
    and $25, $8, $6
    beqz $25, .L80068E50_54
    nop
    addiu $9, $0, -0x1
    xor $7, $7, $9
    xor $6, $6, $9
    addiu $7, $7, 0x1
    sltiu $25, $7, 0x1
    addu $6, $6, $25
.L80068E50_54:
    and $3, $3, $8
    beqz $3, . + 0x118
    nop
    bnez $6, .L80068E50_7C
    nop
    addu $13, $31, $0
    jal func_8006A6F4
    nop
    j func_8006A84C
    addu $31, $13, $0
.L80068E50_7C:
    addu $13, $31, $0
    jal func_8006A5F8
    nop
    addu $31, $13, $0
    addiu $7, $0, -0x1
    xor $3, $3, $7
    xor $2, $2, $7
    addiu $3, $3, 0x1
    sltiu $25, $3, 0x1
    jr $31
    addu $2, $2, $25
    nop
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A870
func_8006A870:
    lui $8, 0x8000
    addu $3, $4, $0
    and $25, $8, $4
    beqz $25, .L80068F00_2C
    nop
    addiu $9, $0, -0x1
    xor $5, $5, $9
    xor $4, $4, $9
    addiu $5, $5, 0x1
    sltiu $25, $5, 0x1
    addu $4, $4, $25
.L80068F00_2C:
    and $25, $8, $6
    beqz $25, .L80068F00_50
    nop
    addiu $9, $0, -0x1
    xor $7, $7, $9
    xor $6, $6, $9
    addiu $7, $7, 0x1
    sltiu $25, $7, 0x1
    addu $6, $6, $25
.L80068F00_50:
    and $3, $3, $8
    beqz $3, . + 0x8C
    nop
    bnez $6, .L80068F00_80
    nop
    addu $13, $31, $0
    jal func_8006A6F4
    nop
    addu $31, $13, $0
    addu $3, $9, $0
    j func_8006A908
    xor $2, $2, $2
.L80068F00_80:
    addu $13, $31, $0
    jal func_8006A5F8
    nop
    addu $31, $13, $0
    addu $3, $5, $0
    addu $2, $4, $0
    addiu $7, $0, -0x1
    xor $3, $3, $7
    xor $2, $2, $7
    addiu $3, $3, 0x1
    sltiu $25, $3, 0x1
    jr $31
    addu $2, $2, $25
    nop
    nop
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A930
func_8006A930:
    bnez $6, 1f
    nop
    j func_8006A6F4
    nop
    1: j func_8006A5F8
    nop
    nop
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A950
func_8006A950:
    bnez $6, 1f
    nop
    addu $13, $31, $0
    jal func_8006A6F4
    nop
    addu $31, $13, $0
    addu $3, $9, $0
    jr $31
    xor $2, $2, $2
    1: addu $13, $31, $0
    jal func_8006A5F8
    nop
    addu $31, $13, $0
    addu $3, $5, $0
    jr $31
    addu $2, $4, $0
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006A990
func_8006A990:
    addiu $29, $29, -0x8
    sdc1 $f12, 0x0($29)
    lw $4, 0x0($29)
    lw $5, 0x4($29)
    addiu $29, $29, 0x8
    addu $3, $4, $0
    srl $4, $4, 20
    andi $4, $4, 0x7FF
    sltiu $25, $4, 0x3FF
    bnez $25, .L80069020_110
    addiu $4, $4, -0x3FF
    sltiu $25, $4, 0x40
    beqz $25, .L80069020_104
    nop
    addu $6, $3, $0
    addu $7, $5, $0
    addu $2, $3, $0
    lui $25, 0xF
    ori $25, $25, 0xFFFF
    and $2, $2, $25
    lui $25, 0x10
    or $2, $2, $25
    sltiu $25, $4, 0x34
    bnez $25, .L80069020_94
    addiu $4, $4, -0x34
    beqz $4, .L80069020_D4
    nop
    addu $3, $4, $0
    addiu $4, $4, -0x20
    negu $4, $4
    addu $5, $7, $0
    srlv $5, $5, $4
    addu $4, $3, $0
    sllv $2, $2, $4
    or $2, $2, $5
    j func_8006AA64
    sllv $7, $7, $4
.L80069020_94:
    negu $4, $4
    addu $3, $4, $0
    sltiu $25, $4, 0x20
    bnez $25, .L80069020_B8
    addiu $4, $4, -0x20
    addu $7, $2, $0
    xor $2, $2, $2
    j func_8006AA64
    srlv $7, $7, $4
.L80069020_B8:
    negu $4, $4
    addu $5, $2, $0
    sllv $5, $5, $4
    addu $4, $3, $0
    srlv $7, $7, $4
    or $7, $7, $5
    srlv $2, $2, $4
.L80069020_D4:
    lui $25, 0x8000
    and $25, $25, $6
    beqz $25, .L80069020_FC
    nop
    addiu $3, $0, -0x1
    xor $7, $7, $3
    xor $2, $2, $3
    addiu $7, $7, 0x1
    sltiu $25, $7, 0x1
    addu $2, $2, $25
.L80069020_FC:
    jr $31
    addu $3, $7, $0
.L80069020_104:
    addiu $3, $0, 0x0
    jr $31
    lui $2, 0x8000
.L80069020_110:
    xor $2, $2, $2
    jr $31
    xor $3, $3, $3
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006AAB0
func_8006AAB0:
addu $3, $5, $0
addu $2, $4, $0
or $4, $4, $5
bnez $4, .Lnz
nop
addiu $sp, $sp, -8
sw $2, 0($sp)
sw $3, 4($sp)
ldc1 $f0, 0($sp)
jr $31
addiu $sp, $sp, 8
.Lnz:
xor $6, $6, $6
lui $4, 0x8000
and $25, $4, $2
beqz $25, .Lpos
nop
addiu $7, $0, -1
xor $3, $3, $7
xor $2, $2, $7
addiu $3, $3, 1
sltiu $25, $3, 1
addu $2, $2, $25
addu $6, $4, $0
.Lpos:
xor $4, $4, $4
lui $25, 0xFFE0
and $25, $25, $2
beqz $25, .Lsmall
nop
.Lshr:
sll $25, $2, 31
srl $3, $3, 1
or $3, $3, $25
srl $2, $2, 1
addiu $4, $4, 1
lui $25, 0xFFE0
and $25, $25, $2
bnez $25, .Lshr
nop
j .Ldone
nop
.Lsmall:
lui $25, 0x10
and $25, $25, $2
bnez $25, .Ldone
nop
srl $25, $3, 31
sll $2, $2, 1
or $2, $2, $25
sll $3, $3, 1
j .Lsmall
addiu $4, $4, -1
.Ldone:
lui $25, 0xF
ori $25, $25, 0xFFFF
and $2, $2, $25
or $2, $2, $6
addiu $4, $4, 0x433
sll $4, $4, 20
or $2, $2, $4
addiu $sp, $sp, -8
sw $2, 0($sp)
sw $3, 4($sp)
ldc1 $f0, 0($sp)
jr $31
addiu $sp, $sp, 8
nop
nop
.type func_8006AAB0, @function
.size func_8006AAB0, 256
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006ABB0
func_8006ABB0:
    addiu $29, $29, -0x8
    swc1 $f12, 0x0($29)
    nop
    lw $4, 0x0($29)
    addiu $29, $29, 0x8
    addu $3, $4, $0
    srl $4, $4, 23
    andi $4, $4, 0xFF
    sltiu $25, $4, 0x7F
    bnez $25, .L80069240_E0
    addiu $4, $4, -0x7F
    sltiu $25, $4, 0x40
    beqz $25, .L80069240_D4
    nop
    addu $6, $3, $0
    lui $25, 0x7F
    ori $25, $25, 0xFFFF
    and $3, $3, $25
    lui $25, 0x80
    or $3, $3, $25
    sltiu $25, $4, 0x17
    bnez $25, .L80069240_98
    addiu $4, $4, -0x17
    beqz $4, .L80069240_A0
    nop
    addu $7, $4, $0
    sltiu $25, $4, 0x20
    bnez $25, .L80069240_80
    addiu $4, $4, -0x20
    xor $3, $3, $3
    j func_8006AC54
    srlv $2, $2, $4
.L80069240_80:
    negu $4, $4
    addu $2, $3, $0
    srlv $2, $2, $4
    addu $4, $7, $0
    j func_8006AC54
    sllv $3, $3, $4
.L80069240_98:
    negu $4, $4
    srlv $3, $3, $4
.L80069240_A0:
    xor $2, $2, $2
    lui $25, 0x8000
    and $25, $25, $6
    beqz $25, .L80069240_CC
    nop
    addiu $6, $0, -0x1
    xor $3, $3, $6
    xor $2, $2, $6
    addiu $3, $3, 0x1
    sltiu $25, $3, 0x1
    addu $2, $2, $25
.L80069240_CC:
    jr $31
    nop
.L80069240_D4:
    addiu $3, $0, 0x0
    jr $31
    lui $2, 0x8000
.L80069240_E0:
    xor $2, $2, $2
    jr $31
    xor $3, $3, $3
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_8006ACA0
func_8006ACA0:
    addu $2, $5, $0
    addu $3, $4, $0
    or $4, $4, $5
    bnez $4, 1f
    nop
    addiu $29, $29, -8
    sw $2, 0($29)
    nop
    lwc1 $f0, 0($29)
    jr $31
    addiu $29, $29, 8
1:  xor $6, $6, $6
    lui $4, 0x8000
    and $25, $4, $3
    beqz $25, 2f
    nop
    addiu $7, $0, -1
    xor $2, $2, $7
    xor $3, $3, $7
    addiu $2, $2, 1
    sltiu $25, $2, 1
    addu $3, $3, $25
    addu $6, $4, $0
2:  addiu $4, $0, 0xB6
    lui $25, 0xFF00
    and $25, $25, $3
    beqz $25, 4f
    nop
3:  sll $25, $3, 31
    srl $2, $2, 1
    or $2, $2, $25
    srl $3, $3, 1
    addiu $4, $4, 1
    lui $25, 0xFF00
    and $25, $25, $3
    bnez $25, 3b
    nop
    j 6f
    nop
4:  lui $25, 0x80
    and $25, $25, $3
    bnez $25, 6f
    nop
    srl $25, $2, 31
    sll $3, $3, 1
    or $3, $3, $25
    sll $2, $2, 1
    j 4b
    addiu $4, $4, -1
6:  lui $2, 0x7F
    ori $2, $2, 0xFFFF
    and $2, $2, $3
    sll $4, $4, 23
    or $2, $2, $4
    or $2, $2, $6
    addiu $29, $29, -8
    sw $2, 0($29)
    nop
    lwc1 $f0, 0($29)
    jr $31
    addiu $29, $29, 8
    nop
    nop
    nop
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef __cmpdi2
.globl __cmpdi2
__cmpdi2 = func_8006A550
#undef __ucmpdi2
.globl __ucmpdi2
__ucmpdi2 = func_8006A590
#undef __muldi3
.globl __muldi3
__muldi3 = func_8006A5C4
#undef __divdi3
.globl __divdi3
__divdi3 = func_8006A7C0
#undef __moddi3
.globl __moddi3
__moddi3 = func_8006A870
#undef __udivdi3
.globl __udivdi3
__udivdi3 = func_8006A930
#undef __umoddi3
.globl __umoddi3
__umoddi3 = func_8006A950
#undef __fixdfdi
.globl __fixdfdi
__fixdfdi = func_8006A990
#undef __floatdidf
.globl __floatdidf
__floatdidf = func_8006AAB0
#undef __fixsfdi
.globl __fixsfdi
__fixsfdi = func_8006ABB0
#undef __floatdisf
.globl __floatdisf
__floatdisf = func_8006ACA0
