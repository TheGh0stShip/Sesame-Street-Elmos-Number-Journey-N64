/* osDestroyThread */
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
    int fp;
    u8 pad1C[4];
    struct {
        u64 at, v0, v1, a0, a1, a2, a3;
        u64 t0, t1, t2, t3, t4, t5, t6, t7;
        u64 s0, s1, s2, s3, s4, s5, s6, s7;
        u64 t8, t9, gp, sp, s8, ra;
        u64 lo, hi;
        u32 sr, pc, cause, badvaddr, rcp;
        u32 fpcsr;
    } context;
} OSThread;
extern OSThread *D_80077C2C; /* __osActiveQueue */
extern OSThread *D_80077C30; /* __osRunningThread */
void func_80059080(OSThread **, OSThread *); /* __osDequeueThread */
void func_80062A2C(void); /* __osDispatchThread */
u32 func_80062C60(void); /* __osDisableInt */
void func_80062C80(u32); /* __osRestoreInt */

void func_80062E70(OSThread *t)
{
    register u32 saveMask;
    register OSThread *pred;
    register OSThread *succ;

    saveMask = func_80062C60();
    if (t == NULL) {
        t = D_80077C30;
    } else if (t->state != 1) {
        func_80059080(t->queue, t);
    }
    if (D_80077C2C == t) {
        D_80077C2C = D_80077C2C->tlnext;
    } else {
        pred = D_80077C2C;
        succ = pred->tlnext;
        while (succ != NULL) {
            if (succ == t) {
                pred->tlnext = t->tlnext;
                break;
            }
            pred = succ;
            succ = pred->tlnext;
        }
    }
    if (t == D_80077C30)
        func_80062A2C();
    func_80062C80(saveMask);
}
