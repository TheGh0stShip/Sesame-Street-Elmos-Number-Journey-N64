/* SDK rmon/rmonbrk.c */

#include "PR/os_internal.h"
#include "PRinternal/dbgproto.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PRinternal/rmonint.h"

#include "PRinternal/macros.h"

// TODO: this comes from a header
#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"

#define TMP_BP 0
#define NUM_BREAKPOINTS 16

typedef struct {
    TVushort type;
    TVushort response;
    TVid threadID;
    void* pc;
} TVExceptionReplyMsg;

typedef struct {
    u32* breakAddress;
    u32 oldInstruction;
} BREAKINFO;
extern /* first breakpoint is reserved for implementing single-stepping */
BREAKINFO breakpoints[NUM_BREAKPOINTS];
extern /* breakpoint for alternate branch target */
BREAKINFO altBreak;
extern BREAKINFO RCPbreakpoints[NUM_BREAKPOINTS];
extern u8 __rmonRcpAtBreak;

void rmonFindFaultedThreads(void);

static void SetTempBreakpoint(u32* addr1, u32* addr2) {
    STUBBED_PRINTF(("Set temp BP at %08x", addr1));
    if (addr2 != NULL) {
        STUBBED_PRINTF((" and %08x", addr2));
    }
    STUBBED_PRINTF(("\n"));

    /* Save the word at the target address to be restored later */
    breakpoints[TMP_BP].oldInstruction = *addr1;
    /* Install a break instruction at the target address */
    *addr1 = MIPS_BREAK(16);
    osWritebackDCache(addr1, sizeof(*addr1));
    osInvalICache(addr1, sizeof(*addr1));
    breakpoints[TMP_BP].breakAddress = addr1;

    /* Also do so for an alt address if required */
    if (addr2 != NULL) {
        altBreak.oldInstruction = *addr2;
        *addr2 = MIPS_BREAK(16);
        osWritebackDCache(addr2, sizeof(*addr2));
        osInvalICache(addr2, sizeof(*addr2));
        altBreak.breakAddress = addr2;
    }
}
void ClearTempBreakpoint(void);
int __rmonSetBreak(KKHeader* req);
int __rmonListBreak(KKHeader* request UNUSED);
int __rmonClearBreak(KKHeader* req);
u32 __rmonGetBranchTarget(int method, int thread, char* addr);

static int IsJump(u32 inst) {
    switch ((inst >> 26) & 0x3F) {
        case 0: /* SPECIAL */
            if (((inst >> 5) & 0x7FFF) == 0 && (inst & 0x3F) == 8) {
                /* JR */
                return TRUE;
            }
            if (((inst >> 16) & 0x1F) == 0 && (inst & 0x7FF) == 9) {
                /* JALR */
                return TRUE;
            }
            break;
        case 2: /* J */
        case 3: /* JAL */
            return TRUE;
    }
    return FALSE;
}

int __rmonSetSingleStep(int thread, u32* instptr) {
    u32 branchTarget = __rmonGetBranchTarget(RMON_CPU, thread, (void*)instptr);

    STUBBED_PRINTF(("SingleStep\n"));

    if ((branchTarget & 3) != 0) {
        /* no branch target, set breakpoint at next pc */
        SetTempBreakpoint(instptr + 1, NULL);
    } else if (branchTarget == (u32)instptr) {
        /* branch target is this instruction, can't single step here */
        return FALSE;
    } else if (IsJump(*instptr) || branchTarget == (u32)(instptr + 2)) {
        /* unconditional branch, set at branch target */
        SetTempBreakpoint((u32*)branchTarget, NULL);
    } else {
        /* set two breakpoints for handling conditional branches */
        SetTempBreakpoint((u32*)branchTarget, instptr + 2);
    }
    return TRUE;
}
void __rmonGetExceptionStatus(KKStatusEvent* reply);

#define FAULT_BREAKNUM (NUM_BREAKPOINTS - 1)

void rmonSendBreakMessage(s32 whichThread, int breakNumber);
void __rmonHitBreak(void);
void __rmonHitSpBreak(void);
void __rmonHitCpuFault(void);
void rmonFindFaultedThreads(void);

