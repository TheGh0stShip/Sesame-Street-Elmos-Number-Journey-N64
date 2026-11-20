/* __rmonSetSRegs (rmonregs.c) */
typedef struct { s32 length; u8 code; u8 pad5; s16 error; u8 pad8; u8 method; u8 padA[2]; } KKHeader;
typedef struct { KKHeader header; s32 tid; u32 sregs[38]; } KKCpScalarRegsetRequest; /* sregs at 0x10 */
typedef struct { KKHeader header; s32 object; } KKObjectEvent;
typedef union {
    u32 everything;
    struct { int opcode : 6; int base : 5; int rt : 5; int offset : 16; } scalarop;
} INSTRUCTION;
extern u32 D_800A7A20; /* RCPpc */
extern u32 D_800A7A24; /* oldIMEMvalue */
extern u32 D_800A7A28[4]; /* DMEMbuffer */
s32 func_800685A0(void); /* __rmonRCPrunning */
u32 func_800659E0(u32 *addr); /* __rmonReadWordAt */
void func_800659A0(u32 *addr, u32 val); /* __rmonWriteWordTo */
void func_80068608(void); /* __rmonStepRCP */
void func_80060898(KKHeader *h, s32 size, s32 type); /* __rmonSendReply */

static void LoadStoreSU(int opcode, int regno) {
    INSTRUCTION inst;
    inst.everything = 0;
    inst.scalarop.opcode = opcode;
    inst.scalarop.rt = regno;
    func_800659A0((u32 *)0x04001000, inst.everything);
    func_800659A0((u32 *)0x04080000, 0);
}
static void SetUpForRCPop(int isVector) {
    D_800A7A20 = func_800659E0((u32 *)0x04080000);
    D_800A7A24 = func_800659E0((u32 *)0x04001000);
    D_800A7A28[0] = func_800659E0((u32 *)0x04000000);
    if (isVector) {
        D_800A7A28[1] = func_800659E0((u32 *)0x04000004);
        D_800A7A28[2] = func_800659E0((u32 *)0x04000008);
        D_800A7A28[3] = func_800659E0((u32 *)0x0400000C);
    }
}
static void CleanupFromRCPop(int isVector) {
    func_800659A0((u32 *)0x04000000, D_800A7A28[0]);
    if (isVector) {
        func_800659A0((u32 *)0x04000004, D_800A7A28[1]);
        func_800659A0((u32 *)0x04000008, D_800A7A28[2]);
        func_800659A0((u32 *)0x0400000C, D_800A7A28[2]);
    }
    func_800659A0((u32 *)0x04001000, D_800A7A24);
    func_800659A0((u32 *)0x04080000, D_800A7A20);
}

int func_800667AC(KKHeader *req) {
    register KKCpScalarRegsetRequest *request = (KKCpScalarRegsetRequest *)req;
    KKObjectEvent reply;
    register int i;

    if (func_800685A0()) {
        return -4;
    }
    SetUpForRCPop(0);
    for (i = 0; i <= 31; i++) {
        func_800659A0((u32 *)0x04000000, request->sregs[i]);
        LoadStoreSU(35, i);
        func_80068608();
    }
    CleanupFromRCPop(0);
    func_800659A0((u32 *)0x04040004, request->sregs[32]);
    func_800659A0((u32 *)0x04040000, request->sregs[33]);
    func_800659A0((u32 *)0x04080000, request->sregs[35] & 0xFFF);
    func_800659A0((u32 *)0x0404000C, request->sregs[36]);
    func_800659A0((u32 *)0x04040010, request->sregs[37]);
    reply.object = request->tid;
    reply.header.code = request->header.code;
    reply.header.error = 0;
    func_80060898(&reply.header, sizeof(reply), 1);
    return 0;
}
