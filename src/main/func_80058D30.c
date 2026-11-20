/* osSetThreadPri: change a thread's priority and reschedule if needed */
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
} OSThread;

extern OSThread *D_80077C30; /* __osRunningThread */
extern OSThread *D_80077C28; /* __osRunQueue */
u32 func_80062C60(void);                          /* __osDisableInt */
void func_80062C80(u32);                          /* __osRestoreInt */
void func_80059080(OSThread **, OSThread *);      /* __osDequeueThread */
void func_800629D4(OSThread **, OSThread *);      /* __osEnqueueThread */
void func_800628CC(OSThread **);                  /* __osEnqueueAndYield */

void func_80058D30(OSThread *t, s32 pri) {
    register u32 saveMask = func_80062C60();

    if (t == NULL) {
        t = D_80077C30;
    }
    if (t->priority != pri) {
        t->priority = pri;
        if (t != D_80077C30 && t->state != 1) {
            func_80059080(t->queue, t);
            func_800629D4(t->queue, t);
        }
        if (D_80077C30->priority < D_80077C28->priority) {
            D_80077C30->state = 2;
            func_800628CC(&D_80077C28);
        }
    }
    func_80062C80(saveMask);
}
