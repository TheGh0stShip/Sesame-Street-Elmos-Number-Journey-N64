/* __rmonHitBreak: clear temporary breakpoints, stop user threads, report faulted threads */
typedef int TVid;
typedef struct {
    int length;
    char code;
    char type;
    short error;
    char rev;
    char method;
    short notused2;
} KKHeader;
typedef struct {
    KKHeader header;
    TVid object;
    unsigned int addr;
} KKSetBkptRequest;
typedef struct {
    KKHeader header;
    TVid object;
    TVid bp;
} KKClrBkptRequest;
typedef struct {
    KKHeader header;
    TVid object;
    TVid bp;
    unsigned int instruction;
} KKBkptEvent;
typedef struct {
    u32 *breakAddress;
    u32 oldInstruction;
} BREAKINFO;
#define NULL_ 0
#define RMON_RSP 1
#define MIPS_BREAK_MASK 0xFC00003F
#define MIPS_BREAK_OPCODE 0xD
#define MIPS_BREAK(code) ((((code) & 0xFFFFF) << 6) | MIPS_BREAK_OPCODE)
extern BREAKINFO D_800A3700[16]; /* breakpoints */
extern BREAKINFO D_800A3780;     /* altBreak */
extern BREAKINFO D_800A3788[16]; /* RCPbreakpoints */
u32 func_800659E0(u32 *addr);             /* __rmonReadWordAt */
void func_800659A0(u32 *addr, u32 val);   /* __rmonWriteWordTo */
void func_80062D90(void *p, s32 n);       /* osWritebackDCache */
void func_80062BE0(void *p, s32 n);       /* osInvalICache */
int func_80060898(KKHeader *h, u32 size, u32 type); /* __rmonSendReply */
void func_80066DEC(int);                  /* __rmonStopUserThreads */
void func_80065708(void);                 /* rmonFindFaultedThreads */
static void ClearTempBreakpoint(void) {
    u32 inst;

    if (D_800A3700[0].breakAddress != NULL) {
        inst = *D_800A3700[0].breakAddress;

        if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
            *D_800A3700[0].breakAddress = D_800A3700[0].oldInstruction;
            func_80062D90(D_800A3700[0].breakAddress, sizeof(*D_800A3700[0].breakAddress));
            func_80062BE0(D_800A3700[0].breakAddress, sizeof(*D_800A3700[0].breakAddress));
        }
        D_800A3700[0].breakAddress = NULL;
    }

    if (D_800A3780.breakAddress != NULL) {
        inst = *D_800A3780.breakAddress;

        if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
            *D_800A3780.breakAddress = D_800A3780.oldInstruction;
            func_80062D90(D_800A3780.breakAddress, sizeof(*D_800A3780.breakAddress));
            func_80062BE0(D_800A3780.breakAddress, sizeof(*D_800A3780.breakAddress));
        }
        D_800A3780.breakAddress = NULL;
    }
}

void func_80065574(void) {
    ClearTempBreakpoint();
    func_80066DEC(0);
    func_80065708();
}
