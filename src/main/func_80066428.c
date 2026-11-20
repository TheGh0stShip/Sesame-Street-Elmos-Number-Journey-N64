/* __rmonGetFRegisters (rmonregs.c) */
typedef struct { int length; u8 code; u8 type; short error; u8 rev; u8 method; /* char is unsigned in this build */ short notused2; } KKHeader;
typedef struct { KKHeader header; s32 object; } KKObjectRequest;
typedef struct { u32 regs[32]; u32 fpcsr; u32 fir; } KKFPregSet;
typedef struct { KKHeader header; s32 tid; KKFPregSet registers; } KKFPregEvent;
typedef struct { u8 pad[0x12C]; u32 fpcsr; u32 fp0; } OSThread;
OSThread *func_80066D88(s32 tid); /* __rmonGetTCB */
void func_80065A50(u32 *dst, u32 *src, s32 n); /* __rmonCopyWords */
s32 func_80060898(KKHeader *h, s32 size, s32 type); /* __rmonSendReply */
s32 func_80066428(KKHeader *req) {
    register KKObjectRequest *request = (KKObjectRequest *)req;
    KKFPregEvent reply;
    OSThread *tptr;
    volatile float f;

    if (req->method != 0) {
        return -2;
    }
    f = 0.0f;
    tptr = func_80066D88(request->object);
    if (tptr == NULL) {
        return -2;
    }
    func_80065A50(reply.registers.regs, &tptr->fp0, 32);
    reply.registers.fpcsr = tptr->fpcsr;
    reply.header.code = request->header.code;
    reply.header.error = 0;
    reply.tid = request->object;
    func_80060898(&reply.header, sizeof(reply), 1);
    return 0;
}
