/* hand-written assembly routine (SDK) */
/* hand-written assembly routine (SDK) */
/* __ptException: libultra 2.0F src/os/exceptasm.s (non-_FINALROM partner-tools exception filter, entered from
   func_80061DF0). Ends with eret and uses $k0/$k1 and CP0 Cause/EPC: no compiler emits this. */
#define C0_CAUSE $13
#define C0_EPC $14
#define CAUSE_IP7 0x4000
#define PI_STATUS_REG 0x04600010
#define PI_STATUS_DMA_BUSY 0x01
#define PI_STATUS_IO_BUSY 0x02
#define PHYS_TO_K1(x) ((x) | 0xA0000000)
#define KMC_CODE_ENTRY 0xBFF00014
#define KMC_WPORT 0xBFF08000
#define KMC_STAT 0xBFF08004
#define __ptException func_80061E10
#define __osException func_80061EB4
#define __osCauseTable_pt D_8007BB24
/* __osException: libultra 2.0F src/os/exceptasm.s, from LEAF(__osException) to END(__osException)
   (non-_FINALROM: RDB/KMC host-interrupt handling present; pre-2.0J cart and PI handlers, pre-2.0K: no
   __osPreviousThread). The general exception handler: runs on $k0/$k1 with interrupts off, saves the whole
   register file, reads/writes CP0 Status/Cause/EPC/Compare/BadVAddr (mfc0/mtc0), cfc1, and returns with
   eret: no compiler produces any of this. Branch targets outside this catalogued range (handle_CpU inside
   send_mesg's range, __osDispatchThreadSave inside __osDispatchThread) are written as '. + offset'. */
#define C0_SR $12
#define C0_COMPARE $11
#define C0_BADVADDR $8
#define fcr31 $31
#define SR_IE 0x00000001
#define SR_EXL 0x00000002
#define SR_IMASK 0x0000ff00
#define SR_IMASKSHIFT 8
#define CAUSE_EXCMASK 0x0000007C
#define CAUSE_SW1 0x00000100
#define CAUSE_SW2 0x00000200
#define CAUSE_IP3 0x00000400
#define CAUSE_IP4 0x00000800
#define CAUSE_IP5 0x00001000
#define CAUSE_IP6 0x00002000
#define CAUSE_IP8 0x00008000
#define EXC_INT 0
#define EXC_BREAK (9 << 2)
#define EXC_CPU (11 << 2)
#define OS_STATE_STOPPED 1
#define OS_STATE_RUNNABLE 2
#define OS_FLAG_CPU_BREAK 1
#define OS_FLAG_FAULT 2
#define MESG(type) ((type) << 3)
#define OS_EVENT_SW1 0
#define OS_EVENT_SW2 1
#define OS_EVENT_CART 2
#define OS_EVENT_COUNTER 3
#define OS_EVENT_SP 4
#define OS_EVENT_SI 5
#define OS_EVENT_AI 6
#define OS_EVENT_VI 7
#define OS_EVENT_PI 8
#define OS_EVENT_DP 9
#define OS_EVENT_CPU_BREAK 10
#define OS_EVENT_SP_BREAK 11
#define OS_EVENT_FAULT 12
#define OS_EVENT_PRENMI 14
#define SP_STATUS_REG 0x04040010
#define SP_CLR_INTR (1 << 3)
#define SP_CLR_SIG3 (1 << 15)
#define SP_STATUS_YIELDED (1 << 8)
#define SP_STATUS_TASKDONE (1 << 9)
#define MI_INIT_MODE_REG 0x04300000
#define MI_CLR_DP_INTR (1 << 11)
#define MI_INTR_REG 0x04300008
#define MI_INTR_MASK_REG 0x0430000C
#define MI_INTR_SP (1 << 0)
#define MI_INTR_SI (1 << 1)
#define MI_INTR_AI (1 << 2)
#define MI_INTR_VI (1 << 3)
#define MI_INTR_PI (1 << 4)
#define MI_INTR_DP (1 << 5)
#define VI_CURRENT_REG 0x04400010
#define AI_STATUS_REG 0x0450000C
#define PI_STATUS_CLR_INTR (1 << 1)
#define SI_STATUS_REG 0x04800018
#define RDB_BASE_REG 0xC0000000
#define RDB_WRITE_INTR_REG (RDB_BASE_REG + 0x8)
#define RDB_READ_INTR_REG (RDB_BASE_REG + 0xc)
#define RDB_TYPE_HtoG_LOG_DONE 13
#define RDB_TYPE_HtoG_DEBUG 14
#define RDB_TYPE_HtoG_DEBUG_CT 15
#define RDB_TYPE_HtoG_DATA 16
#define RDB_TYPE_HtoG_DATA_DONE 17
#define RDB_TYPE_HtoG_REQ_RAMROM 18
#define RDB_TYPE_HtoG_FREE_RAMROM 19
#define RDB_TYPE_HtoG_KDEBUG 20
#define RDB_TYPE_HtoG_PROF_SIGNAL 21
#define HWINTR_SIZE 0x4
#define MQ_MTQUEUE 0
#define THREAD_NEXT 0
#define THREAD_PRI 4
#define THREAD_STATE 16
#define THREAD_FLAGS 18
#define THREAD_FP 24
#define THREAD_GP1 32
#define THREAD_GP2 40
#define THREAD_GP3 48
#define THREAD_GP4 56
#define THREAD_GP5 64
#define THREAD_GP6 72
#define THREAD_GP7 80
#define THREAD_GP8 88
#define THREAD_GP9 96
#define THREAD_GP10 104
#define THREAD_GP11 112
#define THREAD_GP12 120
#define THREAD_GP13 128
#define THREAD_GP14 136
#define THREAD_GP15 144
#define THREAD_GP16 152
#define THREAD_GP17 160
#define THREAD_GP18 168
#define THREAD_GP19 176
#define THREAD_GP20 184
#define THREAD_GP21 192
#define THREAD_GP22 200
#define THREAD_GP23 208
#define THREAD_GP24 216
#define THREAD_GP25 224
#define THREAD_GP28 232
#define THREAD_GP29 240
#define THREAD_GP30 248
#define THREAD_GP31 256
#define THREAD_LO 264
#define THREAD_HI 272
#define THREAD_SR 280
#define THREAD_PC 284
#define THREAD_CAUSE 288
#define THREAD_BADVADDR 292
#define THREAD_RCP 296
#define THREAD_FPCSR 300
#define THREAD_FP0 304
#define THREAD_FP2 312
#define THREAD_FP4 320
#define THREAD_FP6 328
#define THREAD_FP8 336
#define THREAD_FP10 344
#define THREAD_FP12 352
#define THREAD_FP14 360
#define THREAD_FP16 368
#define THREAD_FP18 376
#define THREAD_FP20 384
#define THREAD_FP22 392
#define THREAD_FP24 400
#define THREAD_FP26 408
#define THREAD_FP28 416
#define THREAD_FP30 424
#define ta0 $12
#define send_mesg func_800627DC
#define kdebugserver func_80063248
#define __osEnqueueThread func_800629D4
#define __osDispatchThread func_80062A2C
#define __osThreadSave D_800A8F88
#define __kmc_pt_mode D_800A8F80
#define __osRdb_DbgRead_Ct D_80077FC4
#define __osRdb_Mesg D_80077FC8
#define __os_Kdebug_Pkt D_80077FCC
#define __osRdb_Read_Data_Ct D_80106C98
#define __osRdb_Read_Data_Buf D_8012FEB4
#define __osRdb_DbgRead_Buf D_800A7F04
#define __osRdb_IP6_Ct D_8013ECF4
#define __osRdb_IP6_Empty D_80077FE0
#define __osRdb_IP6_Data D_8013C728
#define __osRdb_IP6_CurSend D_80128574
#define __osRdb_IP6_Size D_80107D18
#define __osRunningThread D_80077C30
#define __OSGlobalIntMask D_80077C10
#define __osRunQueue D_80077C28
#define __osFaultedThread D_80077C34
#define __osShutdown D_80077C0C
#define __osHwIntTable D_80077FB0
#define leoDiskStack D_800A7F80
#define __osIntOffTable D_8007BAE0
#define __osIntTable D_8007BB00
/* hand-written assembly routine (SDK) */
/* hand-written assembly routine (SDK) */
/* __osEnqueueAndYield: libultra 2.0F src/os/exceptasm.s (pre-2.0K: no __osPreviousThread store).
   Saves the running thread's callee-saved context (incl. CP0 Status via mfc0, FPU csr via cfc1, $k0/$k1 use),
   optionally enqueues it, then jumps to __osDispatchThread: register-file save code no compiler emits. */
#define RCP_IMASK 0x003f0000
#define RCP_IMASKSHIFT 16
#define __osEnqueueAndYield func_800628CC
/* __osEnqueueThread (hand-written assembly): insert thread a1 into priority queue a0 */
/* hand-written assembly: *list = (*list)->next */
/* __osDispatchThread (with its __osDispatchThreadSave entry) and __osCleanupThread: libultra 2.0F
   src/os/exceptasm.s (pre-2.0K: no __osThprofFunc hook). Restores the whole register file of the next
   runnable thread, writes CP0 Status/EPC (mtc0), the FPU csr (ctc1) and leaves with eret; __osCleanupThread
   (8 bytes at 0x80062BA8, catalogued inside this range) calls osDestroyThread(NULL). */
#define OS_STATE_RUNNING 4
#define __osPopThread func_80062A1C
#define osDestroyThread func_80062E70
#define __osRcpImTable D_8007B810
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_80061DE0
func_80061DE0:
    lui $26, 0x8006
    addiu $26, $26, 0x1EB4
    jr $26
    nop
.set at
.set reorder

.set noreorder
.set noat
.globl func_80061DF0
func_80061DF0:
    sw $26, -0x10($29)
    beq $0, $0, .L80061DF0_14
    lui $26, 0x8006
    nop
    nop
.L80061DF0_14:
    addiu $26, $26, 0x1E10
    jr $26
    nop
.set at
.set reorder

.text
LEAF(__ptException)
.set noreorder
    mfc0    $k0, C0_CAUSE
    andi    $k0, $k0, 0x7c
    bnez    $k0, pt_not_Int
     srl    $k0, $k0, 2
    mfc0    $k0, C0_CAUSE
    nop
    andi    $k0, $k0, CAUSE_IP7
    beqz    $k0, . + 0x88  /* beqz k0, __osException (next routine, 0x88 bytes on) */
     nop
    j       pt_break
     nop
pt_not_Int:
    sw      $k1, -8($sp)
    /* determine if the pt exception handler should deal with this cause */
    la      $k1, __osCauseTable_pt
    add     $k1, $k1, $k0
    lbu     $k0, ($k1)
    beqz    $k0, . + 0x64  /* beqz k0, __osException */
     nop
pt_break:
pi_ok_loop:
    lw      $k0, PHYS_TO_K1(PI_STATUS_REG)
    nop
    andi    $k0, $k0, (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)
    bnez    $k0, pi_ok_loop
     nop
    lw      $k0, KMC_STAT
    nop
    andi    $k0, $k0, 8
    bnez    $k0, pt_prof
     li     $k0, KMC_CODE_ENTRY
    lw      $k1, -8($sp)
    jr      $k0
     addiu  $sp, $sp, -0x60
pt_prof:
    mfc0    $k0, C0_EPC
    /* a messy way to write \"sw k0, KMC_WPORT\" without using at */
    lui     $k1, (KMC_WPORT >> 16 + 1)
    sw      $k0, -(KMC_WPORT & 0xFFFF)($k1)
    lw      $k1, -8($sp)
    lw      $k0, -0x10($sp)
    eret
    nop
.set reorder
END(__ptException)

.text
LEAF(__osException)
    /* Load scratch space for thread saving */
    la      $k0, __osThreadSave
    /* Save AT */
.set noat
    sd      $1, THREAD_GP1($k0);
.set at
    /* Save SR */
    MFC0($k1, C0_SR)
    sw      $k1, THREAD_SR($k0)
    /* Disable interrupts */
    and     $k1, $k1, ~(SR_IE | SR_EXL)
    MTC0($k1, C0_SR)
    /* Save some temp registers for use in the following */
    sd      $8, THREAD_GP8($k0)
    sd      $9, THREAD_GP9($k0)
    sd      $10, THREAD_GP10($k0)
    /* Mark FPU as unused */
    sw      $zero, THREAD_FP($k0)
    /* This instruction is useless, leftover because of bad placement of an ifdef for the debug version */
    MFC0($t0, C0_CAUSE)
    lw      $t2, __kmc_pt_mode
    bnez    $t2, skip_kmc_mode
    andi    $t1, $t0, 0x7c
    li      $t2, 0
    bne     $t1, $t2, savecontext
    and     $t1, $k1, $t0
    andi    $t2, $t1, CAUSE_IP7
    beqz    $t2, notIP7
    /* clear rdb write interrupt */
    la      $t1, RDB_WRITE_INTR_REG
    sw      $zero, ($t1)
IP7check:
    MFC0($t0, C0_CAUSE)
    andi    $t0, $t0, CAUSE_IP7
    bne     $zero, $t0, IP7check
    la      $t2, RDB_BASE_REG
    lw      $t0, ($t2)
    srl     $t1, $t0, 0x1a
    andi    $t1, $t1, 0x3f
    li      $t2, RDB_TYPE_HtoG_DATA
    beq     $t1, $t2, HandData
    li      $t2, RDB_TYPE_HtoG_DEBUG
    beq     $t1, $t2, HandDbg
    li      $t2, RDB_TYPE_HtoG_KDEBUG
    beq     $t1, $t2, HandKDebug
    li      $t2, RDB_TYPE_HtoG_DEBUG_CT
    beq     $t1, $t2, DbgCnt
    li      $t2, RDB_TYPE_HtoG_DATA_DONE
    beq     $t1, $t2, DataRead
    li      $t2, RDB_TYPE_HtoG_LOG_DONE
    beq     $t1, $t2, LogRead
    li      $t2, RDB_TYPE_HtoG_REQ_RAMROM
    beq     $t1, $t2, ReqRamrom
    li      $t2, RDB_TYPE_HtoG_FREE_RAMROM
    beq     $t1, $t2, FreeRamrom
    li      $t2, RDB_TYPE_HtoG_PROF_SIGNAL
    beq     $t1, $t2, SignalProf
    b       rdbout
SignalProf:
    li      $t2, 1
    srl     $t1, $t0, 0x10
    andi    $t1, $t1, 0xff
    beq     $t1, $t2, AckProf
    li      $t2, 0xa8
    sw      $t2, __osRdb_Mesg
    b       savecontext
AckProf:
    li      $t2, 0xb0
    sw      $t2, __osRdb_Mesg
    b       savecontext
HandKDebug:
    sw      $t0, __os_Kdebug_Pkt
    b       savecontext
DbgCnt:
    li      $t2, 0xffffff
    and     $t1, $t0, $t2
    sw      $t1, __osRdb_DbgRead_Ct
    b       rdbout
DataRead:
    li      $t2, 0x88
    sw      $t2, __osRdb_Mesg
    b       savecontext
LogRead:
    li      $t2, 0x80
    sw      $t2, __osRdb_Mesg
    b       savecontext
ReqRamrom:
    li      $t2, 0x90
    sw      $t2, __osRdb_Mesg
    b       savecontext
FreeRamrom:
    li      $t2, 0x98
    sw      $t2, __osRdb_Mesg
    b       savecontext
HandData:
    srl     $t1, $t0, 0x18
    andi    $t1, $t1, 3
    beq     $zero, $t1, rdbout
    lw      $t2, __osRdb_Read_Data_Ct
    subu    $t2, $t2, $t1
    sw      $t2, __osRdb_Read_Data_Ct
    sd      $t3, THREAD_GP11($k0)
    lw      $t3, __osRdb_Read_Data_Buf
    srl     $t2, $t0, 0x10
    andi    $t2, $t2, 0xff
    sb      $t2, ($t3)
    addi    $t3, $t3, 1
    addi    $t1, $t1, -1
    beq     $zero, $t1, doneData
    srl     $t2, $t0, 8
    andi    $t2, $t2, 0xff
    sb      $t2, ($t3)
    addi    $t3, $t3, 1
    addi    $t1, $t1, -1
    beq     $zero, $t1, doneData
    andi    $t0, $t0, 0xff
    sb      $t0, ($t3)
    addi    $t3, $t3, 1
doneData:
    sw      $t3, __osRdb_Read_Data_Buf
    ld      $t3, THREAD_GP11($k0)
    lw      $t2, __osRdb_Read_Data_Ct
    bne     $zero, $t2, rdbout
    li      $t2, 0x78
    sw      $t2, __osRdb_Mesg
    b       savecontext
HandDbg:
    srl     $t1, $t0, 0x18
    andi    $t1, $t1, 3
    beq     $zero, $t1, rdbout
    lw      $t2, __osRdb_DbgRead_Ct
    subu    $t2, $t2, $t1
    sw      $t2, __osRdb_DbgRead_Ct
    sd      $t3, THREAD_GP11($k0)
    lw      $t3, __osRdb_DbgRead_Buf
    bne     $zero, $t3, 1f
    ld      $t3, THREAD_GP11($k0)
    b       rdbout
1:
    srl     $t2, $t0, 0x10
    andi    $t2, $t2, 0xff
    sb      $t2, ($t3)
    addi    $t3, $t3, 1
    addi    $t1, $t1, -1
    beq     $zero, $t1, doneDbg
    srl     $t2, $t0, 8
    andi    $t2, $t2, 0xff
    sb      $t2, ($t3)
    addi    $t3, $t3, 1
    addi    $t1, $t1, -1
    beq     $zero, $t1, doneDbg
    andi    $t0, $t0, 0xff
    sb      $t0, ($t3)
    addi    $t3, $t3, 1
doneDbg:
    sw      $t3, __osRdb_DbgRead_Buf
    ld      $t3, THREAD_GP11($k0)
    lw      $t2, __osRdb_DbgRead_Ct
    bne     $zero, $t2, rdbout
    li      $t2, 0xa0
    sw      $t2, __osRdb_Mesg
    b       savecontext
notIP7:
    andi    $t2, $t1, CAUSE_IP6
    beqz    $t2, savecontext
    /* clear rdb read interrupt */
    la      $t1, RDB_READ_INTR_REG
    sw      $zero, ($t1)
    lw      $t2, __osRdb_IP6_Ct
    bnez    $t2, 2f
    li      $t2, 1
    sw      $t2, __osRdb_IP6_Empty
    b       rdbout
2:
    addi    $t2, $t2, -1
    sw      $t2, __osRdb_IP6_Ct
    lw      $t0, __osRdb_IP6_Data
    lw      $t1, __osRdb_IP6_CurSend
    sll     $t2, $t1, 2
    add     $t0, $t2, $t0
    lw      $t2, ($t0)
    addi    $t1, $t1, 1
    lw      $t0, __osRdb_IP6_Size
    sub     $t0, $t0, $t1
    bgtz    $t0, 5f
    li      $t1, 0
5:
    sw      $t1, __osRdb_IP6_CurSend
checkIP6:
    MFC0($t0, C0_CAUSE)
    andi    $t0, $t0, CAUSE_IP6
    bne     $zero, $t0, checkIP6
    la      $t0, RDB_BASE_REG
    sw      $t2, ($t0)
rdbout:
    ld      $t0, THREAD_GP8($k0)
    ld      $t1, THREAD_GP9($k0)
    ld      $t2, THREAD_GP10($k0)
.set noat
    ld      $1, THREAD_GP1($k0)
.set at
    lw      $k1, THREAD_SR($k0)
    MTC0($k1, C0_SR)
.set noreorder
    nop
    nop
    nop
    nop
    eret
.set reorder
skip_kmc_mode:
savecontext:
    /* Save the context of the previously running thread to be restored when it resumes */
    move    $t0, $k0
    lw      $k0, __osRunningThread
    ld      $t1, THREAD_GP1($t0)
    sd      $t1, THREAD_GP1($k0)
    ld      $t1, THREAD_SR($t0)
    sd      $t1, THREAD_SR($k0)
    ld      $t1, THREAD_GP8($t0)
    sd      $t1, THREAD_GP8($k0)
    ld      $t1, THREAD_GP9($t0)
    sd      $t1, THREAD_GP9($k0)
    ld      $t1, THREAD_GP10($t0)
    sd      $t1, THREAD_GP10($k0)
3:
    sd      $2, THREAD_GP2($k0)
    sd      $3, THREAD_GP3($k0)
    sd      $4, THREAD_GP4($k0)
    sd      $5, THREAD_GP5($k0)
    sd      $6, THREAD_GP6($k0)
    sd      $7, THREAD_GP7($k0)
    sd      $11, THREAD_GP11($k0)
    sd      $12, THREAD_GP12($k0)
    sd      $13, THREAD_GP13($k0)
    sd      $14, THREAD_GP14($k0)
    sd      $15, THREAD_GP15($k0)
    sd      $16, THREAD_GP16($k0)
    sd      $17, THREAD_GP17($k0)
    sd      $18, THREAD_GP18($k0)
    sd      $19, THREAD_GP19($k0)
    sd      $20, THREAD_GP20($k0)
    sd      $21, THREAD_GP21($k0)
    sd      $22, THREAD_GP22($k0)
    sd      $23, THREAD_GP23($k0)
    sd      $24, THREAD_GP24($k0)
    sd      $25, THREAD_GP25($k0)
    sd      $28, THREAD_GP28($k0)
    sd      $29, THREAD_GP29($k0)
    sd      $30, THREAD_GP30($k0)
    sd      $31, THREAD_GP31($k0)
    mflo    $t0
    sd      $t0, THREAD_LO($k0)
    mfhi    $t0
    sd      $t0, THREAD_HI($k0)
    lw      $k1, THREAD_SR($k0)
    andi    $t1, $k1, SR_IMASK
    beqz    $t1, savercp
    /* If any CPU interrupts are enabled in the SR of the previous thread, bitwise-OR in the disabled CPU interrupts from the global interrupt mask. This is an attempt at reverting the effect of masking the SR of the thread with the global interrupt mask. This is however broken, see comments for func_800584E0. */
    la      $t0, __OSGlobalIntMask
    lw      $t0, 0($t0)
    xor     $t2, $t0, ~0  /* not except not using not */
    andi    $t2, $t2, SR_IMASK
    or      ta0, $t1, $t2
    and     $t3, $k1, ~SR_IMASK
    or      $t3, $t3, ta0
    sw      $t3, THREAD_SR($k0)
    andi    $t0, $t0, SR_IMASK
    and     $t1, $t1, $t0
    and     $k1, $k1, ~SR_IMASK
    or      $k1, $k1, $t1
savercp:
    /* Save the currently masked RCP interrupts. */
    lw      $t1, PHYS_TO_K1(MI_INTR_MASK_REG)
    beqz    $t1, endrcp
    /* Similar to the above comment, but for RCP interrupt enable bits rather than CPU. This suffers from the same problem as above. */
    la      $t0, __OSGlobalIntMask
    lw      $t0, 0($t0)
    srl     $t0, $t0, 0x10
    xor     $t0, $t0, ~0
    andi    $t0, $t0, 0x3f
    lw      ta0, THREAD_RCP($k0)
    and     $t0, $t0, ta0
    or      $t1, $t1, $t0
endrcp:
    sw      $t1, THREAD_RCP($k0)
    MFC0($t0, C0_EPC)
    sw      $t0, THREAD_PC($k0)
    lw      $t0, THREAD_FP($k0)
    beqz    $t0, 1f
    /* Save FP Registers if FPU was used by the thread */
    CFC1($t0, fcr31)
    NOP
    sw      $t0, THREAD_FPCSR($k0)
    sdc1    $f0, THREAD_FP0($k0)
    sdc1    $f2, THREAD_FP2($k0)
    sdc1    $f4, THREAD_FP4($k0)
    sdc1    $f6, THREAD_FP6($k0)
    sdc1    $f8, THREAD_FP8($k0)
    sdc1    $f10, THREAD_FP10($k0)
    sdc1    $f12, THREAD_FP12($k0)
    sdc1    $f14, THREAD_FP14($k0)
    sdc1    $f16, THREAD_FP16($k0)
    sdc1    $f18, THREAD_FP18($k0)
    sdc1    $f20, THREAD_FP20($k0)
    sdc1    $f22, THREAD_FP22($k0)
    sdc1    $f24, THREAD_FP24($k0)
    sdc1    $f26, THREAD_FP26($k0)
    sdc1    $f28, THREAD_FP28($k0)
    sdc1    $f30, THREAD_FP30($k0)
1:
    /* Determine the cause of the exception or interrupt and enter appropriate handling routine */
    MFC0($t0, C0_CAUSE)
    sw      $t0, THREAD_CAUSE($k0)
.set noreorder
    li      $t1, OS_STATE_RUNNABLE
    sh      $t1, THREAD_STATE($k0)
.set reorder
    lw      $a0, __os_Kdebug_Pkt
    beqz    $a0, no_kdebug
    sw      $zero, __os_Kdebug_Pkt
    jal     kdebugserver
    b       . + 0x61C  /* b __osDispatchThreadSave (0x80062A4C) */
no_kdebug:
    lw      $a0, __osRdb_Mesg
    beqz    $a0, no_rdb_mesg
    sw      $zero, __osRdb_Mesg
    jal     send_mesg
    lw      $t0, 0x120($k0)
no_rdb_mesg:
    andi    $t1, $t0, CAUSE_EXCMASK
    /* Test for break exception */
    li      $t2, EXC_BREAK
    beq     $t1, $t2, handle_break
    /* Test for CpU (coprocessor unusable) exception */
    li      $t2, EXC_CPU
    beq     $t1, $t2, . + 0x42C  /* beq t1, t2, handle_CpU (0x80062898) */
    /* Test for interrupt, if it is not an interrupt, panic */
    li      $t2, EXC_INT
    bne     $t1, $t2, panic
handle_interrupt:
    and     $s0, $k1, $t0
next_interrupt:
    /* Handle external interrupt causes, using a jump table to enter into the appropriate handler */
    andi    $t1, $s0, SR_IMASK
    srl     $t2, $t1, 0xc
    bnez    $t2, 1f
    srl     $t2, $t1, SR_IMASKSHIFT
    addi    $t2, $t2, 16
1:
    lbu     $t2, __osIntOffTable($t2)
    lw      $t2, __osIntTable($t2)
    jr      $t2
IP6_Hdlr:
    /* Mask out interrupt and continue */
    and     $s0, $s0, ~CAUSE_IP6
    b       next_interrupt
IP7_Hdlr:
    /* Mask out interrupt and continue */
    and     $s0, $s0, ~CAUSE_IP7
    b       next_interrupt
counter:
    MFC0($t1, C0_COMPARE)
    MTC0($t1, C0_COMPARE)
    li      $a0, MESG(OS_EVENT_COUNTER)
    /* Post counter message */
    jal     send_mesg
    /* Mask out interrupt and continue */
    and     $s0, $s0, ~CAUSE_IP8
    b       next_interrupt
cart:
    li      $a0, MESG(OS_EVENT_CART)
    /* Mask out interrupt */
    and     $s0, $s0, ~CAUSE_IP4
    la      $sp, leoDiskStack
    addiu   $sp, 0x1000 - 0x10  /* Stack size minus initial frame */
    /* Load cart callback set by __osSetHWIntrRoutine */
    li      $t2, HWINTR_SIZE
    lw      $t2, __osHwIntTable($t2)
    /* If the callback is NULL, handling is done */
    beqz    $t2, 1f
    /* Set up a stack and run the callback */
    jalr    $t2
    li      $a0, MESG(OS_EVENT_CART)
    beqz    $v0, 1f
    /* Redispatch immediately if the callback returned nonzero */
    b       redispatch
1:
    /* Post a cart event message */
    jal     send_mesg
    /* Continue */
    b       next_interrupt
rcp:
    /* Load the MI interrupts and mask with the RCP bits in the global interrupt mask @bug this clobbers the t0 register which is expected to hold the value of the C0_CAUSE register in the sw1 and sw2 handlers. If the sw1 or sw2 handler runs after this, the interrupt will not be cleared properly. */
    lw      $s1, PHYS_TO_K1(MI_INTR_REG)
    la      $t0, __OSGlobalIntMask
    lw      $t0, ($t0)
    srl     $t0, $t0, 0x10
    and     $s1, $s1, $t0
    /* Test for sp interrupt */
    andi    $t1, $s1, MI_INTR_SP
    beqz    $t1, vi
    /* Test for yielded or done signals in particular */
    /* Mask out SP interrupt */
    andi    $s1, $s1, (MI_INTR_SI | MI_INTR_AI | MI_INTR_VI | MI_INTR_PI | MI_INTR_DP)
    lw      ta0, PHYS_TO_K1(SP_STATUS_REG)
    li      $t1, (SP_CLR_INTR | SP_CLR_SIG3)
    /* Clear interrupt and signal 3 */
    sw      $t1, PHYS_TO_K1(SP_STATUS_REG)
    andi    ta0, ta0, (SP_STATUS_YIELDED | SP_STATUS_TASKDONE)
    beqz    ta0, sp_other_break
    /* Post an SP event message */
    li      $a0, MESG(OS_EVENT_SP)
    jal     send_mesg
    beqz    $s1, NoMoreRcpInts
    /* Step over sp_other_break handler */
    b       vi
sp_other_break:
    /* An sp signal that is not due to yielding or task completion, such as an sp breakpoint. Post a different event message */
    li      $a0, MESG(OS_EVENT_SP_BREAK)
    jal     send_mesg
    beqz    $s1, NoMoreRcpInts
vi:
    /* Test for vi interrupt */
    andi    $t1, $s1, MI_INTR_VI
    beqz    $t1, ai
    /* Mask out vi interrupt */
    andi    $s1, $s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_AI | MI_INTR_PI | MI_INTR_DP)
    /* Clear interrupt */
    sw      $zero, PHYS_TO_K1(VI_CURRENT_REG)
    /* Post vi event message */
    li      $a0, MESG(OS_EVENT_VI)
    jal     send_mesg
    beqz    $s1, NoMoreRcpInts
ai:
    /* Test for ai interrupt */
    andi    $t1, $s1, MI_INTR_AI
    beqz    $t1, si
    /* Mask out ai interrupt */
    andi    $s1, $s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_VI | MI_INTR_PI | MI_INTR_DP)
    li      $t1, 1
    /* Clear interrupt */
    sw      $t1, PHYS_TO_K1(AI_STATUS_REG)
    /* Post ai event message */
    li      $a0, MESG(OS_EVENT_AI)
    jal     send_mesg
    beqz    $s1, NoMoreRcpInts
si:
    /* Test for si interrupt */
    andi    $t1, $s1, MI_INTR_SI
    beqz    $t1, pi
    /* Mask out si interrupt */
    andi    $s1, $s1, (MI_INTR_SP | MI_INTR_AI | MI_INTR_VI | MI_INTR_PI | MI_INTR_DP)
    /* Clear interrupt */
    sw      $zero, PHYS_TO_K1(SI_STATUS_REG)
    /* Post si event message */
    li      $a0, MESG(OS_EVENT_SI)
    jal     send_mesg
    beqz    $s1, NoMoreRcpInts
pi:
    /* Test for pi interrupt */
    andi    $t1, $s1, MI_INTR_PI
    beqz    $t1, dp
    /* Mask out pi interrupt */
    andi    $s1, $s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_AI | MI_INTR_VI | MI_INTR_DP)
    /* Clear interrupt */
    li      $t1, PI_STATUS_CLR_INTR
    sw      $t1, PHYS_TO_K1(PI_STATUS_REG)
    /* Post pi event message */
    li      $a0, MESG(OS_EVENT_PI)
    jal     send_mesg
    beqz    $s1, NoMoreRcpInts
dp:
    /* Test for dp interrupt */
    andi    $t1, $s1, MI_INTR_DP
    beqz    $t1, NoMoreRcpInts
    /* Mask out dp interrupt */
    andi    $s1, $s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_AI | MI_INTR_VI | MI_INTR_PI)
    /* Clear dp interrupt */
    li      $t1, MI_CLR_DP_INTR
    sw      $t1, PHYS_TO_K1(MI_INIT_MODE_REG)
    /* Post dp event message */
    li      $a0, MESG(OS_EVENT_DP)
    jal     send_mesg
NoMoreRcpInts:
    /* Mask out interrupt and continue */
    and     $s0, $s0, ~CAUSE_IP3
    b       next_interrupt
prenmi:
    /* Disable IP5/PreNMI interrupt for the previously running thread */
    lw      $k1, THREAD_SR($k0)
    and     $k1, $k1, ~CAUSE_IP5
    sw      $k1, THREAD_SR($k0)
    /* Test __osShutdown for first PreNMI event */
    la      $t1, __osShutdown
    lw      $t2, 0($t1)
    beqz    $t2, firstnmi
    /* Mask out interrupt and redispatch immediately */
    and     $s0, $s0, ~CAUSE_IP5
    b       redispatch
firstnmi:
    /* Set __osShutdown */
    li      $t2, 1
    sw      $t2, ($t1)
    /* Post a PreNMI event message */
    li      $a0, MESG(OS_EVENT_PRENMI)
    jal     send_mesg
    /* Mask out and disable IP5/PreNMI interrupt for the highest priority thread */
    and     $s0, $s0, ~CAUSE_IP5
    lw      $t2, __osRunQueue
    lw      $k1, THREAD_SR($t2)
    and     $k1, $k1, ~CAUSE_IP5
    sw      $k1, THREAD_SR($t2)
    /* Redispatch immediately */
    b       redispatch
sw2:
    /* Mask out interrupt */
    and     $t0, $t0, ~CAUSE_SW2
    MTC0($t0, C0_CAUSE)
    /* Post sw2 event message */
    li      $a0, MESG(OS_EVENT_SW2)
    jal     send_mesg
    /* Mask out interrupt and continue */
    and     $s0, $s0, ~CAUSE_SW2
    b       next_interrupt
sw1:
    /* Mask out interrupt */
    and     $t0, $t0, ~CAUSE_SW1
    MTC0($t0, C0_CAUSE)
    /* Post sw1 event message */
    li      $a0, MESG(OS_EVENT_SW1)
    jal     send_mesg
    /* Mask out interrupt and continue */
    and     $s0, $s0, ~CAUSE_SW1
    b       next_interrupt
handle_break:
    /* Set last thread as having hit a break exception */
    li      $t1, OS_FLAG_CPU_BREAK
    sh      $t1, THREAD_FLAGS($k0)
    /* Post a cpu break event message */
    li      $a0, MESG(OS_EVENT_CPU_BREAK)
    jal     send_mesg
    /* Redispatch */
    b       redispatch
redispatch:
    /* Get priority of previously running thread */
    lw      $t1, THREAD_PRI($k0)
    lw      $t2, __osRunQueue
    /* Get highest priority from waiting threads */
    lw      $t3, THREAD_PRI($t2)
    bge     $t1, $t3, enqueueRunning
    /* The previously running thread is no longer the highest priority, enqueue it to the run queue to wait its turn again */
    move    $a1, $k0
    la      $a0, __osRunQueue
    jal     __osEnqueueThread
    j       __osDispatchThread
enqueueRunning:
    la      $t1, __osRunQueue
    lw      $t2, MQ_MTQUEUE($t1)
    sw      $t2, THREAD_NEXT($k0)
    sw      $k0, MQ_MTQUEUE($t1)
    j       __osDispatchThread
panic:
    /* Mark the thread as having faulted */
    sw      $k0, __osFaultedThread
    li      $t1, OS_STATE_STOPPED
    sh      $t1, THREAD_STATE($k0)
    li      $t1, OS_FLAG_FAULT
    sh      $t1, THREAD_FLAGS($k0)
    /* Save C0_BADVADDR */
    MFC0($t2, C0_BADVADDR)
    sw      $t2, THREAD_BADVADDR($k0)
    /* Post the fault message */
    li      $a0, MESG(OS_EVENT_FAULT)
    jal     send_mesg
    /* Dispatch next thread */
    j       __osDispatchThread
END(__osException)

.set noreorder
.set noat
.globl send_mesg
send_mesg:
    addu $18, $31, $0
    lui $10, 0x8013
    addiu $10, $10, 0x3EF8
    addu $10, $10, $4
    lw $9, 0x0($10)
    beqz $9, .L800627DC_B4
    nop
    lw $11, 0x8($9)
    lw $12, 0x10($9)
    slt $1, $11, $12
    beqz $1, .L800627DC_B4
    nop
    lw $13, 0xC($9)
    addu $13, $13, $11
    div $0, $13, $12
    bnez $12, .L800627DC_48
    nop
    break 7
.L800627DC_48:
    addiu $1, $0, -0x1
    bne $12, $1, .L800627DC_60
    lui $1, 0x8000
    bne $13, $1, .L800627DC_60
    nop
    break 6
.L800627DC_60:
    mfhi $13
    lw $12, 0x14($9)
    addiu $1, $0, 0x4
    mult $13, $1
    mflo $13
    addu $12, $12, $13
    lw $13, 0x4($10)
    sw $13, 0x0($12)
    addiu $10, $11, 0x1
    sw $10, 0x8($9)
    lw $10, 0x0($9)
    lw $11, 0x0($10)
    beqz $11, .L800627DC_B4
    nop
    jal __osPopThread
    addu $4, $9, $0
    addu $10, $2, $0
    addu $5, $10, $0
    lui $4, 0x8007
    jal __osEnqueueThread
    addiu $4, $4, 0x7C28
.L800627DC_B4:
    jr $18
    nop
    lui $1, 0x3000
    and $9, $8, $1
    srl $9, $9, 28
    addiu $10, $0, 0x1
    bne $9, $10, . - 0xFC
.set at
.set reorder

.set noreorder
.set noat
.globl func_800628AC
func_800628AC:
    nop
    addiu $9, $0, 0x1
    sw $9, 0x18($26)
    lw $27, 0x118($26)
    lui $1, 0x2000
    or $27, $27, $1
    beq $0, $0, . - 0x130
    sw $27, 0x118($26)
.set at
.set reorder

.text
LEAF(__osEnqueueAndYield)
    lw      $a1, __osRunningThread
    MFC0($t0, C0_SR)
    ori     $t0, $t0, SR_EXL
    sw      $t0, THREAD_SR($a1)
    sd      $s0, THREAD_GP16($a1)
    sd      $s1, THREAD_GP17($a1)
    sd      $s2, THREAD_GP18($a1)
    sd      $s3, THREAD_GP19($a1)
    sd      $s4, THREAD_GP20($a1)
    sd      $s5, THREAD_GP21($a1)
    sd      $s6, THREAD_GP22($a1)
    sd      $s7, THREAD_GP23($a1)
    sd      $gp, THREAD_GP28($a1)
    sd      $sp, THREAD_GP29($a1)
    sd      $s8, THREAD_GP30($a1)
    sd      $ra, THREAD_GP31($a1)
    sw      $ra, THREAD_PC($a1)
    lw      $k1, THREAD_FP($a1)
    beqz    $k1, 1f
    cfc1    $k1, fcr31
    sw      $k1, THREAD_FPCSR($a1)
    sdc1    $f20, THREAD_FP20($a1)
    sdc1    $f22, THREAD_FP22($a1)
    sdc1    $f24, THREAD_FP24($a1)
    sdc1    $f26, THREAD_FP26($a1)
    sdc1    $f28, THREAD_FP28($a1)
    sdc1    $f30, THREAD_FP30($a1)
1:
    lw      $k1, THREAD_SR($a1)
    andi    $t1, $k1, SR_IMASK
    beqz    $t1, 2f
    la      $t0, __OSGlobalIntMask
    lw      $t0, 0($t0)
    xor     $t0, $t0, ~0
    andi    $t0, $t0, SR_IMASK
    or      $t1, $t1, $t0
    and     $k1, $k1, ~SR_IMASK
    or      $k1, $k1, $t1
    sw      $k1, THREAD_SR($a1)
2:
    lw      $k1, PHYS_TO_K1(MI_INTR_MASK_REG)
    beqz    $k1, 3f
    la      $k0, __OSGlobalIntMask
    lw      $k0, 0($k0)
    srl     $k0, $k0, RCP_IMASKSHIFT
    xor     $k0, $k0, ~0
    andi    $k0, $k0, (RCP_IMASK >> RCP_IMASKSHIFT)
    lw      $t0, THREAD_RCP($a1)
    and     $k0, $k0, $t0
    or      $k1, $k1, $k0
3:
    sw      $k1, THREAD_RCP($a1)
    beqz    $a0, noEnqueue
    jal     __osEnqueueThread
noEnqueue:
    j       __osDispatchThread
END(__osEnqueueAndYield)

.set noreorder
.set noat
.globl __osEnqueueThread
__osEnqueueThread:
addu $t9, $a0, $zero
 lw $t8, 0($a0)
 lw $t7, 4($a1)
 lw $t6, 4($t8)
 slt $at, $t6, $t7
 bnez $at, 2f
 nop
1: addu $t9, $t8, $zero
 lw $t8, 0($t8)
 lw $t6, 4($t8)
 slt $at, $t6, $t7
 beqz $at, 1b
 nop
2: lw $t8, 0($t9)
 sw $t8, 0($a1)
 sw $a1, 0($t9)
 jr $ra
 sw $a0, 8($a1)
.set reorder
.set at

.set noreorder
.globl __osPopThread
__osPopThread:
lw $2, 0($4)
lw $25, 0($2)
jr $31
sw $25, 0($4)
.set reorder

.text
LEAF(__osDispatchThread)
    la      $a0, __osRunQueue
    jal     __osPopThread
    sw      $v0, __osRunningThread
    li      $t0, OS_STATE_RUNNING
    sh      $t0, THREAD_STATE($v0)
    move    $k0, $v0
1:
__osDispatchThreadSave:
    lw      $k1, THREAD_SR($k0)
    la      $t0, __OSGlobalIntMask
    lw      $t0, 0($t0)
    andi    $t0, $t0, SR_IMASK
    andi    $t1, $k1, SR_IMASK
    and     $t1, $t1, $t0
    and     $k1, $k1, ~SR_IMASK
    or      $k1, $k1, $t1
    MTC0($k1, C0_SR)
.set noat
    ld      $1, THREAD_GP1($k0)
.set at
    ld      $2, THREAD_GP2($k0)
    ld      $3, THREAD_GP3($k0)
    ld      $4, THREAD_GP4($k0)
    ld      $5, THREAD_GP5($k0)
    ld      $6, THREAD_GP6($k0)
    ld      $7, THREAD_GP7($k0)
    ld      $8, THREAD_GP8($k0)
    ld      $9, THREAD_GP9($k0)
    ld      $10, THREAD_GP10($k0)
    ld      $11, THREAD_GP11($k0)
    ld      $12, THREAD_GP12($k0)
    ld      $13, THREAD_GP13($k0)
    ld      $14, THREAD_GP14($k0)
    ld      $15, THREAD_GP15($k0)
    ld      $16, THREAD_GP16($k0)
    ld      $17, THREAD_GP17($k0)
    ld      $18, THREAD_GP18($k0)
    ld      $19, THREAD_GP19($k0)
    ld      $20, THREAD_GP20($k0)
    ld      $21, THREAD_GP21($k0)
    ld      $22, THREAD_GP22($k0)
    ld      $23, THREAD_GP23($k0)
    ld      $24, THREAD_GP24($k0)
    ld      $25, THREAD_GP25($k0)
    ld      $28, THREAD_GP28($k0)
    ld      $29, THREAD_GP29($k0)
    ld      $30, THREAD_GP30($k0)
    ld      $31, THREAD_GP31($k0)
    ld      $k1, THREAD_LO($k0)
    mtlo    $k1
    ld      $k1, THREAD_HI($k0)
    mthi    $k1
    lw      $k1, THREAD_PC($k0)
    MTC0($k1, C0_EPC)
    lw      $k1, THREAD_FP($k0)
    beqz    $k1, 1f
    lw      $k1, THREAD_FPCSR($k0)
    CTC1($k1, fcr31)
    ldc1    $f0, THREAD_FP0($k0)
    ldc1    $f2, THREAD_FP2($k0)
    ldc1    $f4, THREAD_FP4($k0)
    ldc1    $f6, THREAD_FP6($k0)
    ldc1    $f8, THREAD_FP8($k0)
    ldc1    $f10, THREAD_FP10($k0)
    ldc1    $f12, THREAD_FP12($k0)
    ldc1    $f14, THREAD_FP14($k0)
    ldc1    $f16, THREAD_FP16($k0)
    ldc1    $f18, THREAD_FP18($k0)
    ldc1    $f20, THREAD_FP20($k0)
    ldc1    $f22, THREAD_FP22($k0)
    ldc1    $f24, THREAD_FP24($k0)
    ldc1    $f26, THREAD_FP26($k0)
    ldc1    $f28, THREAD_FP28($k0)
    ldc1    $f30, THREAD_FP30($k0)
1:
.set noreorder
    lw      $k1, THREAD_RCP($k0)
    la      $k0, __OSGlobalIntMask
    lw      $k0, 0($k0)
    srl     $k0, $k0, 0x10
    and     $k1, $k1, $k0
    sll     $k1, $k1, 0x1
    la      $k0, __osRcpImTable
    addu    $k1, $k1, $k0
    lhu     $k1, 0($k1)
    la      $k0, PHYS_TO_K1(MI_INTR_MASK_REG)
    sw      $k1, 0($k0)
    nop
    nop
    nop
    nop
    eret
.set reorder
END(__osDispatchThread)
LEAF(__osCleanupThread)
    move    $a0, $zero
    nop
    jal     osDestroyThread
END(__osCleanupThread)

/* known names (symbols.number.tsv) */
#undef __osExceptionPreamble
.globl __osExceptionPreamble
__osExceptionPreamble = func_80061DE0
#undef __ptExceptionPreamble
.globl __ptExceptionPreamble
__ptExceptionPreamble = func_80061DF0
#undef __ptException
.globl __ptException
__ptException = func_80061E10
#undef __osException
.globl __osException
__osException = func_80061EB4
#undef send_mesg
.globl send_mesg
send_mesg = func_800627DC
#undef __osEnqueueAndYield
.globl __osEnqueueAndYield
__osEnqueueAndYield = func_800628CC
#undef __osEnqueueThread
.globl __osEnqueueThread
__osEnqueueThread = func_800629D4
#undef __osPopThread
.globl __osPopThread
__osPopThread = func_80062A1C
#undef __osDispatchThread
.globl __osDispatchThread
__osDispatchThread = func_80062A2C
