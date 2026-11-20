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

int __rmonListThreads(KKHeader* req) {
    register KKObjectRequest* request = (KKObjectRequest*)req;
    KKObjsEvent* reply = (KKObjsEvent*)__rmonUtilityBuffer;

    STUBBED_PRINTF(("ListThreads\n"));

    reply->object = (request->object == -1) ? RMON_PID_CPU : request->object;

    if (req->method == RMON_RSP) {
        reply->objs.number = 1;
        reply->objs.objects[0] = RMON_TID_RSP;
    } else {
        register OSThread* tptr = __osGetActiveQueue();

        reply->objs.number = 0;

        while (tptr->priority != -1) {
            if (tptr->id != 0) {
                reply->objs.objects[reply->objs.number] = tptr->id;
                reply->objs.number++;
            }
            tptr = tptr->tlnext;
        }
    }
    reply->header.code = request->header.code;
    reply->header.error = TV_ERROR_NO_ERROR;
    __rmonSendReply(&reply->header, sizeof(*reply) + sizeof(reply->objs.objects[0]) * (reply->objs.number - 1),
                    KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}
int __rmonGetThreadStatus(int method, int id, KKStatusEvent* reply);
int __rmonThreadStatus(KKHeader* req);
int __rmonStopThread(KKHeader* req);
int __rmonRunThread(KKHeader* req);

