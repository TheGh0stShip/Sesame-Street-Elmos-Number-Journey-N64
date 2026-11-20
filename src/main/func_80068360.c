/* osStopThread (libultra 2.0F src/os/stopthread.c) */
typedef struct OSThread {
    struct OSThread *next;
    s32 priority;
    struct OSThread **queue;
    struct OSThread *tlnext;
    u16 state;
    u16 flags;
} OSThread;
extern OSThread *D_80077C30; /* __osRunningThread */
u32 func_80062C60(void); /* __osDisableInt */
void func_80062C80(u32); /* __osRestoreInt */
void func_800628CC(OSThread **); /* __osEnqueueAndYield */
void func_80059080(OSThread **, OSThread *); /* __osDequeueThread */

void func_80068360(OSThread *t) {
    register u32 saveMask = func_80062C60();
    register u16 state;

    state = (t == NULL) ? 4 : t->state;

    switch (state) {
        case 4:
            D_80077C30->state = 1;
            func_800628CC(NULL);
            break;
        case 2:
        case 8:
            t->state = 1;
            func_80059080(t->queue, t);
            break;
    }
    func_80062C80(saveMask);
}
