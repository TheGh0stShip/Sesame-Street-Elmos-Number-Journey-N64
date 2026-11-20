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

int __rmonClearBreak(KKHeader* req) {
    register KKClrBkptRequest* request = (KKClrBkptRequest*)req;
    register BREAKINFO* whichBreak;
    KKBkptEvent reply;
    u32 inst;

    STUBBED_PRINTF(("ClearBreak\n"));

    /* Check valid breakpoint index */
    if (request->bp >= NUM_BREAKPOINTS) {
        return TV_ERROR_INVALID_ID;
    }

    /* Clear the breakpoint, restore whatever was there before */
    if (req->method == RMON_RSP) {
        whichBreak = &RCPbreakpoints[request->bp];

        if (whichBreak->breakAddress == NULL) {
            return TV_ERROR_INVALID_ID;
        }

        inst = __rmonReadWordAt(whichBreak->breakAddress);
        if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
            __rmonWriteWordTo(whichBreak->breakAddress, whichBreak->oldInstruction);
        }
    } else {
        whichBreak = &breakpoints[request->bp];

        if (whichBreak->breakAddress == NULL) {
            return TV_ERROR_INVALID_ID;
        }

        inst = *whichBreak->breakAddress;
        if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
            *whichBreak->breakAddress = whichBreak->oldInstruction;
            osWritebackDCache(whichBreak->breakAddress, sizeof(*whichBreak->breakAddress));
            osInvalICache(whichBreak->breakAddress, sizeof(*whichBreak->breakAddress));
        }
    }
    whichBreak->breakAddress = NULL;

    /* Send reply */
    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.object = request->object;
    reply.bp = request->bp;
    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}
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

