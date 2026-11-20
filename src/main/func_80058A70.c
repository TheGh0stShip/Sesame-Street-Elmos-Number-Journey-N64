/* osRecvMesg */
typedef void *OSMesg;
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
} OSThread;
typedef struct {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
extern OSThread *D_80077C30; /* __osRunningThread */
u32 func_80062C60(void);                        /* __osDisableInt */
void func_80062C80(u32);                        /* __osRestoreInt */
OSThread *func_80062A1C(OSThread **);           /* __osPopThread */
void func_800628CC(OSThread **);                /* __osEnqueueAndYield */
void func_80058EA0(OSThread *);                 /* osStartThread */

s32 func_80058A70(OSMesgQueue *mq, OSMesg *msg, s32 flags)
{
    register u32 saveMask;

    saveMask = func_80062C60();
    while (mq->validCount == 0) {
        if (flags == 0) {
            func_80062C80(saveMask);
            return -1;
        }
        D_80077C30->state = 8;
        func_800628CC(&mq->mtqueue);
    }
    if (msg != NULL)
        *msg = mq->msg[mq->first];
    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;
    if (mq->fullqueue->next != NULL)
        func_80058EA0(func_80062A1C(&mq->fullqueue));
    func_80062C80(saveMask);
    return 0;
}
