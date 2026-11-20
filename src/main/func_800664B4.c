/* __rmonSetFRegisters: copy FP registers from a debugger request into a thread's context */
typedef struct { s32 unk_00; u8 code; u8 pad; s16 error; } KKHeader;
typedef struct { KKHeader header; u8 unk_08; u8 method; u8 pad3[2]; s32 tid; u32 regs[32]; s32 fpcsr; } KKFPRegsetRequest;
typedef struct { KKHeader header; s32 unk_08; s32 object; } KKObjectEvent;
typedef struct { u8 pad[0x12C]; u32 fpcsr; u32 fp0[32]; } OSThread;
extern OSThread *func_80066D88(s32);                 /* __rmonGetTCB */
extern void func_80065A50(u32 *, u32 *, s32);        /* __rmonCopyWords */
extern void func_80060898(KKHeader *, s32, s32);     /* __rmonSendReply */
s32 func_800664B4(KKHeader *req)
{
    register KKFPRegsetRequest *request = (KKFPRegsetRequest *)req;
    KKObjectEvent reply;
    OSThread *tptr;
    volatile f32 f;

    if (request->method != 0) {
        return -2;
    }
    f = 0.0f;
    tptr = func_80066D88(request->tid);
    if (tptr == NULL) {
        return -2;
    }
    func_80065A50(tptr->fp0, request->regs, 32);
    tptr->fpcsr = request->fpcsr;
    reply.object = request->tid;
    reply.header.code = request->header.code;
    reply.header.error = 0;
    func_80060898(&reply.header, 0x10, 1);
    return 0;
}
