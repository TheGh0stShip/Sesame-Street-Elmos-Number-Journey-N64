/* osStartThread */
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
} OSThread;
extern OSThread *D_80077C28; /* __osRunQueue */
extern OSThread *D_80077C30; /* __osRunningThread */
u32 func_80062C60(void);                        /* __osDisableInt */
void func_80062C80(u32);                        /* __osRestoreInt */
void func_800629D4(OSThread **, OSThread *);    /* __osEnqueueThread */
OSThread *func_80062A1C(OSThread **);           /* __osPopThread */
void func_80062A2C(void);                       /* __osDispatchThread */
void func_800628CC(OSThread **);                /* __osEnqueueAndYield */

void func_80058EA0(OSThread *t)
{
    register u32 saveMask = func_80062C60();

    switch (t->state) {
    case 8:
        t->state = 2;
        func_800629D4(&D_80077C28, t);
        break;
    case 1:
        if (t->queue == NULL || t->queue == &D_80077C28) {
            t->state = 2;
            func_800629D4(&D_80077C28, t);
        } else {
            t->state = 8;
            func_800629D4(t->queue, t);
            func_800629D4(&D_80077C28, func_80062A1C(t->queue));
        }
        break;
    }
    if (D_80077C30 == NULL) {
        func_80062A2C();
    } else if (D_80077C30->priority < D_80077C28->priority) {
        D_80077C30->state = 2;
        func_800628CC(&D_80077C28);
    }
    func_80062C80(saveMask);
}
