/* send a fixed event message to its queue without blocking (interrupt-side osSendMesg) */
typedef void *OSMesg;
typedef struct OSThread_s { struct OSThread_s *next; } OSThread;
typedef struct {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct {
    OSMesgQueue *messageQueue;
    OSMesg message;
} __OSEventState;
extern __OSEventState D_80133F38;
extern OSThread *D_80077C28; /* __osRunQueue */
OSThread *func_80062A1C(OSThread **); /* __osPopThread */
void func_800629D4(OSThread **, OSThread *); /* __osEnqueueThread */

void func_80064EBC(void)
{
    OSMesgQueue *mq = D_80133F38.messageQueue;

    if (mq != NULL && mq->validCount < mq->msgCount) {
        mq->msg[(mq->first + mq->validCount) % mq->msgCount] = D_80133F38.message;
        mq->validCount++;
        if (mq->mtqueue->next != NULL)
            func_800629D4(&D_80077C28, func_80062A1C(&mq->mtqueue));
    }
}
