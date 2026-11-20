#include "common.h"

/* libultra rmon __rmonSetVRegs: load RSP vector registers by stepping LQV instructions */
typedef struct {
    s32 length;
    u8 code;
    u8 unk_05;
    s16 error;
    s32 method;
    s32 object;
} KKHeader;
typedef struct {
    KKHeader header;
    u32 vregs[32];
} KKVregRequest;
typedef union {
    u32 everything;
    struct {
        unsigned opcode : 6;
        unsigned base : 5;
        unsigned rt : 5;
        unsigned opc : 5;
        unsigned element : 4;
        unsigned offset : 7;
    } swcop;
} INSTRUCTION; /* __rmonRCPrunning */
extern u32 __rmonReadWordAt(u32 addr); /* __rmonReadWordAt */
extern void __rmonWriteWordTo(u32 addr, u32 val);/* __rmonWriteWordTo */
/* __rmonMemcpy */
 /* __rmonStepRCP */
extern void __rmonSendReply(void *reply, s32 size, s32 flag); /* __rmonSendReply */
extern u32 D_800A7A20, D_800A7A24, DMEMbuffer, D_800A7A2C, D_800A7A30, D_800A7A34;

s32 __rmonSetVRegs(KKVregRequest *req) {
    KKHeader reply;
    INSTRUCTION inst;
    s32 i;

    if (__rmonRCPrunning()) {
        return -4;
    }
    D_800A7A20 = __rmonReadWordAt(0x04080000);
    D_800A7A24 = __rmonReadWordAt(0x04001000);
    DMEMbuffer = __rmonReadWordAt(0x04000000);
    D_800A7A2C = __rmonReadWordAt(0x04000004);
    D_800A7A30 = __rmonReadWordAt(0x04000008);
    D_800A7A34 = __rmonReadWordAt(0x0400000C);
    for (i = 0; i < 32; i++) {
        __rmonCopyWords(0x04000000, &req->vregs[i], 4);
        inst.everything = 0xC8000000;
        inst.swcop.rt = i;
        inst.swcop.opc = 4;
        __rmonWriteWordTo(0x04001000, inst.everything);
        __rmonWriteWordTo(0x04080000, 0);
        __rmonStepRCP();
    }
    __rmonWriteWordTo(0x04000000, DMEMbuffer);
    __rmonWriteWordTo(0x04000004, D_800A7A2C);
    __rmonWriteWordTo(0x04000008, D_800A7A30);
    __rmonWriteWordTo(0x0400000C, D_800A7A30);
    __rmonWriteWordTo(0x04001000, D_800A7A24);
    __rmonWriteWordTo(0x04080000, D_800A7A20);
    reply.object = req->header.object;
    reply.code = req->header.code;
    reply.error = 0;
    __rmonSendReply(&reply, sizeof(reply), 1);
    return 0;
}
