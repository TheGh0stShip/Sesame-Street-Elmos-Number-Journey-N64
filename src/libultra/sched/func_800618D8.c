#include <ultralog.h>
#include <assert.h>
#include <sched.h>
/* osSendMesg */
/* osViBlack */
 /* osViSwapBuffer */
s32 __scTaskComplete(OSSched *sc, OSScTask *t)
{
    int rv;
    static int firsttime = 1;
    if ((t->state & 3) == 0) {
        t->totalTime += osGetTime() - t->startTime;
        rv = osSendMesg(t->msgQ, t->msg, 1);
        if (t->list.t.type == 1) {
            if ((t->flags & 0x40) && (t->flags & 0x20)) {
                if (firsttime) {
                    osViBlack(0);
                    firsttime = 0;
                }
                osViSwapBuffer(t->framebuffer);
            }
        }
        return 1;
    }
    return 0;
}
