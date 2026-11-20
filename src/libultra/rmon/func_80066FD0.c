/* SDK rmon/rmontask.c */

#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PRinternal/rmonint.h"
#include "PR/rcp.h"
#include "PR/sptask.h"

#include "PRinternal/macros.h"

// TODO: this comes from a header
#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"

void __rmonMaskIdleThreadInts(void);
OSThread* __rmonGetTCB(int threadNumber);
int __rmonStopUserThreads(int whichThread);
int __rmonListThreads(KKHeader* req);

int __rmonGetThreadStatus(int method, int id, KKStatusEvent* reply) {
    u32 inst;

    STUBBED_PRINTF(("ThreadStatus %d method %d\n", id, method));

    reply->status.tid = id;
    reply->status.pid = (method == RMON_RSP) ? RMON_PID_RSP : RMON_PID_CPU;
    reply->status.why = 1;
    reply->status.what = 0;
    reply->status.info.major = 0;
    reply->status.info.minor = 0;
    reply->status.rv = 0;

    if (method == RMON_RSP) {
        reply->status.start = SP_IMEM_START;
        reply->status.priority = RMON_PRI_RSP;

        if (__rmonRCPrunning()) {
            reply->status.flags = OS_STATE_RUNNING;
            /* Cannot read RSP PC or current instruction while the RSP is running */
            reply->status.info.addr = 0;
            reply->status.instr = 0;
        } else {
            reply->status.flags = OS_STATE_STOPPED;
            reply->status.info.addr = __rmonReadWordAt((u32*)SP_PC_REG) + SP_IMEM_START;
            inst = __rmonReadWordAt((u32*)reply->status.info.addr);
            if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
                inst = MIPS_BREAK_OPCODE;
            }
            if (__rmonRcpAtBreak) {
                /* Report RSP break */
                reply->status.why = 2;
                reply->status.info.major = 2;
                reply->status.info.minor = 4;
            }
            reply->status.instr = inst;
        }
    } else {
        OSThread* tptr = __osGetActiveQueue();

        while (tptr->priority != -1) {
            if (tptr->id == id) {
                break;
            }
            tptr = tptr->tlnext;
        }
        if (tptr->priority == -1) {
            return TV_ERROR_INVALID_ID;
        }

        reply->status.priority = tptr->priority;
        reply->status.flags = (tptr->state != 0) ? tptr->state : OS_STATE_STOPPED;
        reply->status.info.addr = tptr->context.pc;

        inst = *(u32*)(tptr->context.pc);
        if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
            inst = MIPS_BREAK_OPCODE;
        }

        reply->status.instr = inst;
        reply->status.start = (int)tptr;

        if (tptr->flags & OS_FLAG_CPU_BREAK) {
            /* Report break */
            reply->status.why = 2;
            reply->status.info.major = 2;
            reply->status.info.minor = 4;
        } else if (tptr->flags & OS_FLAG_FAULT) {
            /* Report fault */
            reply->status.why = 2;
            reply->status.info.major = 1;
            reply->status.info.minor = 2;
        }
    }

    return TV_ERROR_NO_ERROR;
}
int __rmonThreadStatus(KKHeader* req);
int __rmonStopThread(KKHeader* req);
int __rmonRunThread(KKHeader* req);

