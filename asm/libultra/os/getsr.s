/* __osGetSR: libultra 2.0F src/os/getsr.s. mfc0 of CP0 Status: not expressible in C. */
#define C0_SR $12
#define __osGetSR func_80062BD0
#include "asm.h"

.text
.text
LEAF(__osGetSR)
    MFC0($v0, C0_SR)
    jr      $ra
END(__osGetSR)
