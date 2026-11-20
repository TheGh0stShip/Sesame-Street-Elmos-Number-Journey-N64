/* osCreateThread */
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
void func_80062BA8(void); /* __osCleanupThread */
u32 func_80062C60(void); /* __osDisableInt */
void func_80062C80(u32); /* __osRestoreInt */

void func_800585E0(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 p)
{
    register u32 saveMask;
    u32 mask;

    t->id = id;
    t->priority = p;
    t->next = NULL;
    t->queue = NULL;
    t->context.pc = (u32)entry;
    t->context.a0 = (s64)(s32)arg;
    t->context.sp = (s64)(s32)sp - 16;
    t->context.ra = (s64)(s32)func_80062BA8;
    mask = 0x003FFF01;
    t->context.sr = (mask & (0xFF00 | 1)) | 2;
    t->context.rcp = (mask & 0x003F0000) >> 16;
    t->context.fpcsr = 0x01000000 | 0x00000800 | 0;
    t->fp = 0;
    t->state = 1;
    t->flags = 0;
    saveMask = func_80062C60();
    t->tlnext = D_80077C2C;
    D_80077C2C = t;
    func_80062C80(saveMask);
}
