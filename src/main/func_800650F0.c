/* __rmonClearBreak: remove a CPU or RSP breakpoint and reply */
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
int func_800650F0(KKHeader *req) {
    register KKClrBkptRequest *request = (KKClrBkptRequest *)req;
    register BREAKINFO *whichBreak;
    KKBkptEvent reply;
    u32 inst;

    if (request->bp >= 16) {
        return -2;
    }

    if (req->method == RMON_RSP) {
        whichBreak = &D_800A3788[request->bp];

        if (whichBreak->breakAddress == NULL) {
            return -2;
        }

        inst = func_800659E0(whichBreak->breakAddress);
        if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
            func_800659A0(whichBreak->breakAddress, whichBreak->oldInstruction);
        }
    } else {
        whichBreak = &D_800A3700[request->bp];

        if (whichBreak->breakAddress == NULL) {
            return -2;
        }

        inst = *whichBreak->breakAddress;
        if ((inst & MIPS_BREAK_MASK) == MIPS_BREAK_OPCODE) {
            *whichBreak->breakAddress = whichBreak->oldInstruction;
            func_80062D90(whichBreak->breakAddress, sizeof(*whichBreak->breakAddress));
            func_80062BE0(whichBreak->breakAddress, sizeof(*whichBreak->breakAddress));
        }
    }
    whichBreak->breakAddress = NULL;

    reply.header.code = request->header.code;
    reply.header.error = 0;
    reply.object = request->object;
    reply.bp = request->bp;
    func_80060898(&reply.header, sizeof(reply), 1);
    return 0;
}
