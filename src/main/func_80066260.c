/* __rmonGetGRegisters: reply with a thread's general registers */
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

s32 func_80066260(KKHeader *req) {
    register KKObjectRequest *request = (KKObjectRequest *)req;
    KKGregEvent reply;

    reply.tid = request->object;
    reply.header.code = request->header.code;
    reply.header.error = 0;

    if (request->header.method == 0) {
        OSThread *tptr = func_80066D88(request->object);
        u64 *tcbregptr;
        register s32 i;

        if (tptr == NULL)
            return -2;
        for (i = 1, tcbregptr = &tptr->context.at; i < 26; i++, tcbregptr++)
            reply.registers.gregs[i] = *tcbregptr;
        for (i = 28, tcbregptr = &tptr->context.gp; i < 34; i++, tcbregptr++)
            reply.registers.gregs[i] = *tcbregptr;
        reply.registers.gregs[34] = tptr->context.cause;
        reply.registers.gregs[35] = tptr->context.pc;
        reply.registers.gregs[36] = tptr->context.sr;
        reply.registers.gregs[0] = 0;
    } else {
        return -2;
    }
    func_80060898(&reply.header, sizeof(reply), 1);
    return 0;
}
