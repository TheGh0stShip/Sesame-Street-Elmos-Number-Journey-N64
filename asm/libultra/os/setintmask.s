/* osSetIntMask: libultra 2.0F src/os/setintmask.s. Reads/writes CP0 Status (mfc0/mtc0) and MI_INTR_MASK_REG through __osRcpImTable. */
#define C0_SR $12
#define OS_IM_CPU 0xFF01
#define SR_IMASK 0xFF00
#define RCP_IMASK 0x003F0000
#define RCP_IMASKSHIFT 16
#define MI_INTR_MASK_REG 0x0430000C
#define PHYS_TO_K1(x) ((x) | 0xA0000000)
#define __OSGlobalIntMask D_80077C10
#define __osRcpImTable D_8007B810
#define osSetIntMask func_800584E0
#include "asm.h"

.text
.text
.set noreorder
LEAF(osSetIntMask)
    mfc0    $t4, C0_SR  /* ta0 */
    andi    $v0, $t4, OS_IM_CPU
    la      $t0, __OSGlobalIntMask
    lw      $t3, 0($t0)
    xor     $t0, $t3, ~0
    andi    $t0, $t0, SR_IMASK
    or      $v0, $v0, $t0
    lw      $t2, PHYS_TO_K1(MI_INTR_MASK_REG)
    beqz    $t2, 1f
     srl    $t1, $t3, 0x10
    xor     $t1, $t1, ~0
    andi    $t1, $t1, (RCP_IMASK >> RCP_IMASKSHIFT)
    or      $t2, $t2, $t1
1:
    sll     $t2, $t2, RCP_IMASKSHIFT
    or      $v0, $v0, $t2
    and     $t0, $a0, RCP_IMASK
    and     $t0, $t0, $t3
    srl     $t0, $t0, (RCP_IMASKSHIFT - 1)
    lhu     $t2, __osRcpImTable($t0)
    sw      $t2, PHYS_TO_K1(MI_INTR_MASK_REG)
    andi    $t0, $a0, OS_IM_CPU
    andi    $t1, $t3, SR_IMASK
    and     $t0, $t0, $t1
    and     $t4, $t4, ~SR_IMASK
    or      $t4, $t4, $t0
    mtc0    $t4, C0_SR
    nop
    nop
    jr      $ra
     nop
END(osSetIntMask)

/* known names (symbols.number.tsv) */
#undef osSetIntMask
.globl osSetIntMask
osSetIntMask = func_800584E0
