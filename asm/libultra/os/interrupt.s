/* __osDisableInt: libultra 2.0F src/os/interrupt.s (pre-2.0J form, no __OSGlobalIntMask re-check). Clears SR_IE in CP0 Status with mfc0/mtc0 and returns the old IE bit. */
#define C0_SR $12
#define SR_IE 0x00000001
#define __osDisableInt func_80062C60
/* __osRestoreInt: libultra 2.0F src/os/interrupt.s. ORs the saved IE bit back into CP0 Status (mfc0/mtc0 + hazard nops). */
#define __osRestoreInt func_80062C80
#include "asm.h"

.text
.text
.set noreorder
LEAF(__osDisableInt)
    mfc0    $t0, C0_SR
    and     $t1, $t0, ~SR_IE
    mtc0    $t1, C0_SR
    andi    $v0, $t0, SR_IE
    nop
    jr      $ra
     nop
END(__osDisableInt)

.text
.set noreorder
LEAF(__osRestoreInt)
    mfc0    $t0, C0_SR
    or      $t0, $t0, $a0
    mtc0    $t0, C0_SR
    nop
    nop
    jr      $ra
     nop
END(__osRestoreInt)

/* known names (symbols.number.tsv) */
#undef __osDisableInt
.globl __osDisableInt
__osDisableInt = func_80062C60
#undef __osRestoreInt
.globl __osRestoreInt
__osRestoreInt = func_80062C80
