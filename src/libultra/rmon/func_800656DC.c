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
void SetTempBreakpoint(u32* addr1, u32* addr2);
void ClearTempBreakpoint(void);
int __rmonSetBreak(KKHeader* req);
int __rmonListBreak(KKHeader* request UNUSED);
int __rmonClearBreak(KKHeader* req);
u32 __rmonGetBranchTarget(int method, int thread, char* addr);
int IsJump(u32 inst);
int __rmonSetSingleStep(int thread, u32* instptr);
void __rmonGetExceptionStatus(KKStatusEvent* reply);

#define FAULT_BREAKNUM (NUM_BREAKPOINTS - 1)

void rmonSendBreakMessage(s32 whichThread, int breakNumber);
void __rmonHitBreak(void);
void __rmonHitSpBreak(void);

void __rmonHitCpuFault(void) {
    STUBBED_PRINTF(("HitCpuFault\n"));

    /* Stop all user threads and report faulted threads */
    __rmonMaskIdleThreadInts();
    __rmonStopUserThreads(0);
    rmonFindFaultedThreads();
}
void rmonFindFaultedThreads(void);

