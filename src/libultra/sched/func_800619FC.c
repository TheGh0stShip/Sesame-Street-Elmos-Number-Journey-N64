#include <ultralog.h>
#include <assert.h>
#include <sched.h>
/* osSpTaskLoad */
/* osSpTaskStartGo */
 /* osDpSetNextBuffer */
static int dp_busy = 0;
static int dpCount = 0;
void __scExec(OSSched *sc, OSScTask *sp, OSScTask *dp)
{
    int rv;
    if (sp) {
        if (sp->list.t.type == 2) {
            osWritebackDCacheAll();
        }
        sp->state &= ~(0x10 | 0x20);
        sp->startTime = osGetTime();
        osSpTaskLoad(&sp->list);
        osSpTaskStartGo(&sp->list);
        sc->curRSPTask = sp;
        if (sp == dp)
            sc->curRDPTask = dp;
    }
    if (dp && (dp != sp)) {
        rv = osDpSetNextBuffer(dp->list.t.output_buff, *dp->list.t.output_buff_size);
        dp_busy = 1;
        dpCount = 0;
        sc->curRDPTask = dp;
    }
}
