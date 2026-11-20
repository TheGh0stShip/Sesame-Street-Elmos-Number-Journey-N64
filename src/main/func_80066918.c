/* __rmonGetVRegs (libultra rmon/rmonregs.c): send the RSP vector registers to the host */
typedef struct {
    s32 length;
    s8 code;
    s8 type;
    s16 error;
    s8 rev;
    u8 method;
    u8 unk_A[2];
} KKHeader;

typedef struct {
    KKHeader header;
    s32 object;
} KKObjectRequest;

typedef struct {
    KKHeader header;
    s32 tid;
    u8 registers[0x200];
} KKCpVregEvent;

typedef union {
    u32 everything;
    struct {
        int opcode : 6;
        int base : 5;
        int rt : 5;
        int size : 5;
        int element : 4;
        int offset : 7;
    } vectorop;
} INSTRUCTION;

extern u32 D_800A7A20;     /* RCPpc */
extern u32 D_800A7A24;     /* oldIMEMvalue */
extern u32 D_800A7A28[4];  /* DMEMbuffer */

s32 func_800685A0(void);                    /* __rmonRCPrunning */
s32 func_80063440(char *, u32, u32);        /* __osRdbSend */
void func_80060830(KKHeader *, s32, s32);   /* __rmonSendHeader */
u32 func_800659E0(u32 *);                   /* __rmonReadWordAt */
void func_800659A0(u32 *, u32);             /* __rmonWriteWordTo */
void func_80068608(void);                   /* __rmonStepRCP */
void func_80060950(void *, s32);            /* __rmonSendData */

static void LoadStoreVU(int opcode, int regno) {
    INSTRUCTION inst;

    inst.everything = 0;
    inst.vectorop.opcode = opcode;
    inst.vectorop.rt = regno;
    inst.vectorop.size = 4;
    func_800659A0((u32 *)0x04001000, inst.everything);
    func_800659A0((u32 *)0x04080000, 0);
}

static void SetUpForRCPop(int isVector) {
    D_800A7A20 = func_800659E0((u32 *)0x04080000);
    D_800A7A24 = func_800659E0((u32 *)0x04001000);
    D_800A7A28[0] = func_800659E0((u32 *)0x04000000);
    if (isVector) {
        D_800A7A28[1] = func_800659E0((u32 *)(0x04000000 + 0x4));
        D_800A7A28[2] = func_800659E0((u32 *)(0x04000000 + 0x8));
        D_800A7A28[3] = func_800659E0((u32 *)(0x04000000 + 0xC));
    }
}

static void CleanupFromRCPop(int isVector) {
    func_800659A0((u32 *)0x04000000, D_800A7A28[0]);
    if (isVector) {
        func_800659A0((u32 *)(0x04000000 + 0x4), D_800A7A28[1]);
        func_800659A0((u32 *)(0x04000000 + 0x8), D_800A7A28[2]);
        /* SDK bug: the last word is not restored properly */
        func_800659A0((u32 *)(0x04000000 + 0xC), D_800A7A28[2]);
    }
    func_800659A0((u32 *)0x04001000, D_800A7A24);
    func_800659A0((u32 *)0x04080000, D_800A7A20);
}

int func_80066918(KKHeader *req) {
    char *cPtr;
    int sent;
    int dataSize;
    register KKObjectRequest *request = (KKObjectRequest *)req;
    KKCpVregEvent reply;
    register int i;

    if (func_800685A0()) {
        return -4;
    }

    reply.tid = request->object;
    reply.header.code = request->header.code;
    reply.header.error = 0;
    reply.header.length = sizeof(reply);

    dataSize = sizeof(reply);
    cPtr = (char *)&dataSize;
    sent = 0;
    while (sent < (signed)sizeof(dataSize)) {
        sent += func_80063440(cPtr + sent, sizeof(dataSize) - sent, 8);
    }

    func_80060830(&reply.header, 16, 1);

    SetUpForRCPop(1);
    for (i = 0; i < 32; i++) {
        LoadStoreVU(0x3A, i);
        func_80068608();
        func_80060950((void *)0x04000000, 16);
    }
    CleanupFromRCPop(1);

    return 0;
}
