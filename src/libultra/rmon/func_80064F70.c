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

int __rmonSetBreak(KKHeader* req) {
    register KKSetBkptRequest* request = (KKSetBkptRequest*)req;
    register BREAKINFO* breakBase;
    register BREAKINFO* whichBreak;
    register BREAKINFO* lastBreak;
    KKBkptEvent reply;

    STUBBED_PRINTF(("SetBreak at %08x, method %d\n", request->addr, req->method));

    /* Select breakpoint list */
    if (req->method == RMON_RSP) {
        breakBase = RCPbreakpoints;
        whichBreak = &RCPbreakpoints[1];
        lastBreak = &RCPbreakpoints[NUM_BREAKPOINTS];
    } else {
        breakBase = breakpoints;
        whichBreak = &breakpoints[1];
        lastBreak = &breakpoints[NUM_BREAKPOINTS];
    }

    /* Find breakpoint slot */
    for (; whichBreak < lastBreak; whichBreak++) {
        if (whichBreak->breakAddress != NULL) {
            if (whichBreak->breakAddress == (u32*)request->addr) {
                /* Breakpoint already set here */
                break;
            }
            continue;
        } else {
            /* Empty slot */
            break;
        }
    }

    /* No breakpoints available */
    if (whichBreak == lastBreak) {
        return TV_ERROR_NO_MORE_IDS;
    }

    /* Set breakpoint if not already set */
    if (whichBreak->breakAddress == NULL) {
        if (req->method == RMON_RSP) {
            whichBreak->oldInstruction = __rmonReadWordAt((u32*)request->addr);
            __rmonWriteWordTo((u32*)request->addr, MIPS_BREAK((whichBreak - breakBase) + NUM_BREAKPOINTS));
        } else {
            whichBreak->oldInstruction = *(u32*)request->addr;
            *(u32*)request->addr = MIPS_BREAK((whichBreak - breakBase) + NUM_BREAKPOINTS);
            osWritebackDCache((void*)request->addr, sizeof(whichBreak->oldInstruction));
            osInvalICache((void*)request->addr, sizeof(whichBreak->oldInstruction));
        }
        whichBreak->breakAddress = (u32*)request->addr;
        STUBBED_PRINTF(("* (%08x) = %08x (was %08x)\n", whichBreak->breakAddress, *whichBreak->breakAddress,
                        whichBreak->oldInstruction));
    }

    /* Send reply */
    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.object = request->object;
    reply.bp = whichBreak - breakBase;
    reply.instruction = whichBreak->oldInstruction;
    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}
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
void __rmonHitCpuFault(void);
void rmonFindFaultedThreads(void);

