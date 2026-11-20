/* __rmonSetBreak: install a CPU or RSP breakpoint and reply with its slot */
typedef int TVid;
typedef struct {
    int length;
    u8 code;
    u8 type;
    short error;
    u8 rev;
    u8 method;
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
int func_80064F70(KKHeader *req) {
    register KKSetBkptRequest *request = (KKSetBkptRequest *)req;
    register BREAKINFO *breakBase;
    register BREAKINFO *whichBreak;
    register BREAKINFO *lastBreak;
    KKBkptEvent reply;

    if (req->method == RMON_RSP) {
        breakBase = D_800A3788;
        whichBreak = &D_800A3788[1];
        lastBreak = &D_800A3788[16];
    } else {
        breakBase = D_800A3700;
        whichBreak = &D_800A3700[1];
        lastBreak = &D_800A3700[16];
    }

    for (; whichBreak < lastBreak; whichBreak++) {
        if (whichBreak->breakAddress != NULL) {
            if (whichBreak->breakAddress == (u32 *)request->addr) {
                break;
            }
            continue;
        } else {
            break;
        }
    }

    if (whichBreak == lastBreak) {
        return -10;
    }

    if (whichBreak->breakAddress == NULL) {
        if (req->method == RMON_RSP) {
            whichBreak->oldInstruction = func_800659E0((u32 *)request->addr);
            func_800659A0((u32 *)request->addr, MIPS_BREAK((whichBreak - breakBase) + 16));
        } else {
            whichBreak->oldInstruction = *(u32 *)request->addr;
            *(u32 *)request->addr = MIPS_BREAK((whichBreak - breakBase) + 16);
            func_80062D90((void *)request->addr, sizeof(whichBreak->oldInstruction));
            func_80062BE0((void *)request->addr, sizeof(whichBreak->oldInstruction));
        }
        whichBreak->breakAddress = (u32 *)request->addr;
    }

    reply.header.code = request->header.code;
    reply.header.error = 0;
    reply.object = request->object;
    reply.bp = whichBreak - breakBase;
    reply.instruction = whichBreak->oldInstruction;
    func_80060898(&reply.header, sizeof(reply), 1);
    return 0;
}
