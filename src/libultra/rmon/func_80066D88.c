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

OSThread* __rmonGetTCB(int threadNumber) {
    register OSThread* tptr = __osGetActiveQueue();

    if (threadNumber < 1) {
        return NULL;
    }

    while (tptr->priority != -1) {
        if (tptr->id == threadNumber) {
            return tptr;
        }
        tptr = tptr->tlnext;
    }

    return NULL;
}
int __rmonStopUserThreads(int whichThread);
int __rmonListThreads(KKHeader* req);
int __rmonGetThreadStatus(int method, int id, KKStatusEvent* reply);
int __rmonThreadStatus(KKHeader* req);
int __rmonStopThread(KKHeader* req);
int __rmonRunThread(KKHeader* req);

