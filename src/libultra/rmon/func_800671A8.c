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

int __rmonThreadStatus(KKHeader* req) {
    KKObjectRequest* request = (KKObjectRequest*)req;
    KKStatusEvent reply;

    if (__rmonGetThreadStatus(req->method, request->object, &reply) != TV_ERROR_NO_ERROR) {
        return TV_ERROR_INVALID_ID;
    }

    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}
int __rmonStopThread(KKHeader* req);
int __rmonRunThread(KKHeader* req);

