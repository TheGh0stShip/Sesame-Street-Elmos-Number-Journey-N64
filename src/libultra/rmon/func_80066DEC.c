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

int __rmonStopUserThreads(int whichThread) {
    register int whichOne = 0;
    register OSThread* tptr = __osGetActiveQueue();

    STUBBED_PRINTF(("StopThreads %d\n", whichThread));

    if (whichThread != 0) {
        /* Stop specified thread */

        while (tptr->priority != -1) {
            if (tptr->id == whichThread) {
                break;
            }
            tptr = tptr->tlnext;
        }

        if (tptr->priority == -1) {
            return 0;
        }

        if (tptr->priority > OS_PRIORITY_IDLE && tptr->priority <= OS_PRIORITY_APPMAX) {
            osStopThread(tptr);
            if (tptr->state != OS_STATE_STOPPED) {
                STUBBED_PRINTF(("Couldn't stop thread %d\n", tptr->id));
            }
            whichOne = whichThread;
        }
    } else {
        /* Stop all threads */

        while (tptr->priority != -1) {
            if (tptr->priority > OS_PRIORITY_IDLE && tptr->priority <= OS_PRIORITY_APPMAX) {
                osStopThread(tptr);
                if (tptr->state != OS_STATE_STOPPED) {
                    STUBBED_PRINTF(("Couldn\'t stop thread %d\n", tptr->id));
                }
                whichOne = -1;
            }
            tptr = tptr->tlnext;
        }
    }
    return whichOne;
}
int __rmonListThreads(KKHeader* req);
int __rmonGetThreadStatus(int method, int id, KKStatusEvent* reply);
int __rmonThreadStatus(KKHeader* req);
int __rmonStopThread(KKHeader* req);
int __rmonRunThread(KKHeader* req);

