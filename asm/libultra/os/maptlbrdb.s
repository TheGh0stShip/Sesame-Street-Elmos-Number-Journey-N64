/* osMapTLBRdb: libultra 2.0F src/os/maptlbrdb.s. Writes TLB entry 31 mapping K2BASE to the RDB port (mtc0 Index/PageMask/EntryHi/EntryLo, tlbwi). */
#define C0_INX $0
#define C0_ENTRYLO0 $2
#define C0_ENTRYLO1 $3
#define C0_PAGEMASK $5
#define C0_ENTRYHI $10
#define NTLBENTRIES 31
#define TLBLO_UNCACHED 0x10
#define TLBLO_D 0x4
#define TLBLO_V 0x2
#define TLBLO_G 0x1
#define TLBLO_PFNSHIFT 6
#define K2BASE 0xC0000000
#define RDB_BASE_VIRTUAL_ADDR 0x80000000
#define osMapTLBRdb func_80062E10
#include "asm.h"

.text
.text
.set noreorder
LEAF(osMapTLBRdb)
    mfc0    $t0, C0_ENTRYHI
    li      $t1, NTLBENTRIES
    mtc0    $t1, C0_INX
    mtc0    $zero, C0_PAGEMASK  /* 4k */
    li      $t2, (TLBLO_UNCACHED | TLBLO_D | TLBLO_V | TLBLO_G)
    li      $t1, K2BASE
    mtc0    $t1, C0_ENTRYHI
    li      $t1, RDB_BASE_VIRTUAL_ADDR
    srl     $t3, $t1, TLBLO_PFNSHIFT
    or      $t3, $t3, $t2
    mtc0    $t3, C0_ENTRYLO0
    li      $t1, TLBLO_G
    mtc0    $t1, C0_ENTRYLO1
    nop
    tlbwi
    nop
    nop
    nop
    nop
    mtc0    $t0, C0_ENTRYHI
    jr      $ra
     nop
END(osMapTLBRdb)

/* known names (symbols.number.tsv) */
#undef osMapTLBRdb
.globl osMapTLBRdb
osMapTLBRdb = func_80062E10
