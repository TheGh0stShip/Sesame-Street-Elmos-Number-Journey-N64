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

u32 __rmonGetBranchTarget(int method, int thread, char* addr) {
    int inst;

    if (method == RMON_RSP) {
        inst = __rmonReadWordAt((u32*)addr);
    } else {
        inst = *(u32*)addr;
    }

    switch ((inst >> 26) & 0x3F) {
        case 0: /* SPECIAL */
            if (((inst >> 5) & 0x7FFF) == 0 && (inst & 0x3F) == 8) {
                /* JR */
                return __rmonGetRegisterContents(method, thread, (inst >> 21) & 0x1F);
            }
            if (((inst >> 16) & 0x1F) == 0 && (inst & 0x7FF) == 9) {
                /* JALR */
                return __rmonGetRegisterContents(method, thread, (inst >> 21) & 0x1F);
            }
            break;
        case 1: /* REGIMM */
            switch ((inst >> 16) & 0x1F) {
                case 0:  /* BLTZ */
                case 1:  /* BGEZ */
                case 2:  /* BLTZL */
                case 3:  /* BGEZL */
                case 16: /* BLTZAL */
                case 17: /* BGEZAL */
                case 18: /* BLTZALL */
                case 19: /* BGEZALL */
                    return (((inst << 0x10) >> 0xE) + addr + 4);
            }
            break;
        case 2: /* J */
        case 3: /* JAL */
            return (((u32)inst << 6) >> 4) + (((s32)((u32)addr + 4) >> 0x1C) << 0x1C);
        case 4:  /* BEQ */
        case 5:  /* BNE */
        case 20: /* BEQL */
        case 21: /* BNEL */
            return (((inst << 0x10) >> 0xE) + addr + 4);
        case 6:  /* BLEZ */
        case 7:  /* BGTZ */
        case 22: /* BLEZL */
        case 23: /* BGTZL */
            if (((inst >> 16) & 0x1F) == 0) {
                return (((inst << 0x10) >> 0xE) + addr + 4);
            }
            break;
        case 16: /* COP0 */
        case 17: /* COP1 */
        case 18: /* COP2 */
        case 19: /* COP3 */
            if (((inst >> 21) & 0x1F) == 8) {
                switch ((inst >> 16) & 0x1F) {
                    case 0: /* BCzF  */
                    case 1: /* BCzT  */
                    case 2: /* BCzFL */
                    case 3: /* BCzTL */
                        return (((inst << 0x10) >> 0xE) + addr + 4);
                }
            }
            break;
    }
    return -1;
}
int IsJump(u32 inst);
int __rmonSetSingleStep(int thread, u32* instptr);
void __rmonGetExceptionStatus(KKStatusEvent* reply);

#define FAULT_BREAKNUM (NUM_BREAKPOINTS - 1)

void rmonSendBreakMessage(s32 whichThread, int breakNumber);
void __rmonHitBreak(void);
void __rmonHitSpBreak(void);
void __rmonHitCpuFault(void);
void rmonFindFaultedThreads(void);

