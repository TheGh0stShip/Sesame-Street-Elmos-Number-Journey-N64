/* SDK io/leointerrupt.c */
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
#include "PRinternal/piint.h"
#include "PRinternal/macros.h"

extern OSPiHandle *__osDiskHandle;
extern u8 leoDiskStack[OS_PIM_STACKSIZE];

void __osLeoAbnormalResume(void);
void __osLeoResume(void);
s32 __osLeoInterrupt(void);
void __osLeoAbnormalResume(void);

void __osLeoResume(void) {
    __OSEventState *es = &__osEventStateTab[OS_EVENT_PI];
    OSMesgQueue *mq = es->messageQueue;
    s32 last;
    
    if (mq == NULL || MQ_IS_FULL(mq)) {
        return;
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = es->message;
    mq->validCount++;
    
    if (mq->mtqueue->next != NULL) {
        __osEnqueueThread(&__osRunQueue, __osPopThread(&mq->mtqueue));
    }
}
