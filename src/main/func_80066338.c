/* __rmonSetGRegisters: load a thread's general registers from a request */
typedef struct {
    s32 length;
    u8 code;
    u8 type;
    s16 error;
    u8 rev;
    u8 method;
    s16 notused;
} KKHeader;
typedef struct { KKHeader header; s32 object; } KKObjectRequest;
typedef struct { u32 gregs[37]; } KKGregSet;
typedef struct { KKHeader header; s32 tid; KKGregSet registers; } KKGregEvent;
typedef struct {
    u64 at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, gp, sp, s8, ra, lo, hi;
    u32 sr, pc, cause, badvaddr, rcp, fpcsr;
} __OSThreadContext;
typedef struct OSThread_s {
    struct OSThread_s *next; s32 priority; void *queue; struct OSThread_s *tlnext;
    u16 state; u16 flags; s32 id; s32 fp; s32 pad;
    __OSThreadContext context;
} OSThread;
OSThread *func_80066D88(s32 id);                         /* __rmonGetTCB */
s32 func_80060898(KKHeader *h, s32 size, s32 type);      /* __rmonSendReply */

typedef struct { KKHeader header; s32 tid; KKGregSet registers; } KKGRegsetRequest;
typedef struct { KKHeader header; s32 object; } KKObjectEvent;

s32 func_80066338(KKHeader *req) {
    register KKGRegsetRequest *request = (KKGRegsetRequest *)req;
    KKObjectEvent reply;

    if (request->header.method == 0) {
        OSThread *tptr = func_80066D88(request->tid);
        u64 *tcbregptr;
        register int i;

        if (tptr == NULL)
            return -2;
        for (i = 1, tcbregptr = &tptr->context.at; i < 26; i++, tcbregptr++)
            *tcbregptr = (s32)request->registers.gregs[i];
        for (i = 28, tcbregptr = &tptr->context.gp; i < 34; i++, tcbregptr++)
            *tcbregptr = (s32)request->registers.gregs[i];
        tptr->context.cause = request->registers.gregs[34];
        tptr->context.pc = request->registers.gregs[35];
        tptr->context.sr = request->registers.gregs[36];
    } else {
        return -2;
    }
    reply.object = request->tid;
    reply.header.code = request->header.code;
    reply.header.error = 0;
    func_80060898(&reply.header, sizeof(reply), 1);
    return 0;
}
