/* osSendMesg */
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

s32 func_80058BA0(OSMesgQueue *mq, OSMesg msg, s32 flags)
{
    register u32 saveMask;
    register s32 last;

    saveMask = func_80062C60();
    while (mq->validCount >= mq->msgCount) {
        if (flags == 1) {
            D_80077C30->state = 8;
            func_800628CC(&mq->fullqueue);
        } else {
            func_80062C80(saveMask);
            return -1;
        }
    }
    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;
    if (mq->mtqueue->next != NULL)
        func_80058EA0(func_80062A1C(&mq->mtqueue));
    func_80062C80(saveMask);
    return 0;
}
