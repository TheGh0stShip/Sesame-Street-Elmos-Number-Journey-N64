/* __osProbeTLB: libultra 2.0F src/os/probetlb.s. Virtual-to-physical lookup via tlbp/tlbr and CP0 EntryHi/Index/PageMask/EntryLo moves. */
#define C0_INX $0
#define C0_ENTRYLO0 $2
#define C0_ENTRYLO1 $3
#define C0_PAGEMASK $5
#define C0_ENTRYHI $10
#define TLBHI_PIDMASK 0xFF
#define TLBHI_VPN2MASK 0xFFFFE000
#define TLBINX_PROBE 0x80000000
#define TLBLO_V 0x2
#define TLBLO_PFNMASK 0x3FFFFFC0
#define TLBLO_PFNSHIFT 6
#define __osProbeTLB func_80062CA0
#include "asm.h"

.text
.text
.set noreorder
LEAF(__osProbeTLB)
    /* Set C0_ENTRYHI based on supplied vaddr */
    mfc0    $t0, C0_ENTRYHI
    andi    $t1, $t0, TLBHI_PIDMASK
    and     $t2, $a0, -0x2000  /* (TLBHI_VPN2MASK << 32) >> 32 */
    or      $t1, $t1, $t2
    mtc0    $t1, C0_ENTRYHI
    nop
    nop
    nop
    tlbp
    nop
    nop
    mfc0    $t3, C0_INX
    and     $t3, $t3, TLBINX_PROBE
    bnez    $t3, 3f
     nop
    tlbr
    nop
    nop
    nop
    /* page size = (page mask + 0x2000) >> 1 */
    mfc0    $t3, C0_PAGEMASK
    addi    $t3, $t3, 0x2000
    srl     $t3, $t3, 0x1
    and     $t4, $t3, $a0  /* ta0 */
    bnez    $t4, 1f
     addi   $t3, $t3, -1
    mfc0    $v0, C0_ENTRYLO0
    b       2f
     nop
1:
    mfc0    $v0, C0_ENTRYLO1
2:
    andi    $t5, $v0, TLBLO_V  /* ta1 */
    beqz    $t5, 3f
     nop
    and     $v0, $v0, TLBLO_PFNMASK
    sll     $v0, $v0, TLBLO_PFNSHIFT
    and     $t5, $a0, $t3
    add     $v0, $v0, $t5
    b       4f
     nop
3:
    li      $v0, -1
4:
    mtc0    $t0, C0_ENTRYHI
    jr      $ra
     nop
END(__osProbeTLB)

/* known names (symbols.number.tsv) */
#undef __osProbeTLB
.globl __osProbeTLB
__osProbeTLB = func_80062CA0
