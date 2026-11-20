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
int __rmonGetThreadStatus(int method, int id, KKStatusEvent* reply);
int __rmonThreadStatus(KKHeader* req);
int __rmonStopThread(KKHeader* req);

int __rmonRunThread(KKHeader* req) {
    KKRunThreadRequest* request = (KKRunThreadRequest*)req;
    KKObjectEvent reply;
    KKStatusEvent exceptionReply;
    register OSThread* tptr;
    register int runNeeded = FALSE;

    STUBBED_PRINTF(("RunThread %d\n", request->tid));

    switch (req->method) {
        case RMON_CPU:
            tptr = __osGetActiveQueue();
            while (tptr->priority != -1) {
                if (tptr->id == request->tid) {
                    break;
                }
                tptr = tptr->tlnext;
            }

            if (tptr->priority == -1) {
                return TV_ERROR_INVALID_ID;
            }
            if (tptr->state != OS_STATE_STOPPED) {
                return TV_ERROR_OPERATIONS_PROTECTED;
            }
            tptr->flags &= ~(OS_FLAG_CPU_BREAK | OS_FLAG_FAULT);
            if (request->actions.flags & KK_RUN_SETPC) {
                tptr->context.pc = request->actions.vaddr;
            }
            if ((request->actions.flags & KK_RUN_SSTEP) && !__rmonSetSingleStep(request->tid, (u32*)tptr->context.pc)) {
                return TV_ERROR_OPERATIONS_PROTECTED;
            }
            runNeeded = TRUE;
            break;
        case RMON_RSP:
            if (__rmonRCPrunning()) {
                return TV_ERROR_OPERATIONS_PROTECTED;
            }
            if (request->actions.flags & KK_RUN_SETPC) {
                __rmonWriteWordTo((u32*)SP_PC_REG, request->actions.vaddr - SP_IMEM_START);
            }
            if (request->actions.flags & KK_RUN_SSTEP) {
                /* If the RSP is stopped at a branch step twice so as to not stop in a branch delay slot. */
                if (__rmonGetBranchTarget(RMON_RSP, RMON_TID_RSP,
                                          (void*)(__rmonReadWordAt((u32*)SP_PC_REG) + SP_IMEM_START)) %
                        4 ==
                    0) {
                    __rmonStepRCP();
                }
                __rmonStepRCP();
                __rmonRcpAtBreak = TRUE;
            } else {
                __rmonRcpAtBreak = FALSE;
                __rmonRunRCP();
            }
            reply.header.code = request->header.code;
            reply.header.error = TV_ERROR_NO_ERROR;
            reply.object = request->tid;
            __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
            if (request->actions.flags & KK_RUN_SSTEP) {
                __rmonGetThreadStatus(RMON_RSP, RMON_TID_RSP, &exceptionReply);
                __rmonGetExceptionStatus(&exceptionReply);
                __rmonSendReply(&exceptionReply.header, sizeof(exceptionReply), KK_TYPE_EXCEPTION);
            }
            return TV_ERROR_NO_ERROR;
        default:
            return TV_ERROR_OPERATIONS_PROTECTED;
    }

    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.object = request->tid;
    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);

    if (runNeeded) {
        osStartThread(tptr);
    }
    return 1;
}

