/* rmonGetRcpRegister: read an RSP scalar register by stepping a single SW instruction */
extern u32 D_800A7A20; /* RCPpc */
extern u32 D_800A7A24; /* oldIMEMvalue */
extern u32 D_800A7A28[4]; /* DMEMbuffer */
s32 func_800685A0(void);                 /* __rmonRCPrunning */
u32 func_800659E0(u32 *addr);            /* __rmonReadWordAt */
void func_800659A0(u32 *addr, u32 val);  /* __rmonWriteWordTo */
void func_80068608(void);                /* __rmonStepRCP */

typedef union {
    u32 everything;
    struct { int opcode : 6; int base : 5; int rt : 5; int offset : 16; } scalarop;
} INSTRUCTION;

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

u32 func_80066540(int regNumber) {
    u32 contents;

    if (func_800685A0())
        return 0;
    SetUpForRCPop(0);
    LoadStoreSU(0x2B, regNumber);
    func_80068608();
    contents = func_800659E0((u32 *)0x04000000);
    CleanupFromRCPop(0);
    return contents;
}
