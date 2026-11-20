/* osCreateViManager (vimgr.c) */
typedef s32 OSPri;
typedef void *OSMesg;
typedef struct OSMesgQueue_s { u8 data[0x18]; } OSMesgQueue;
typedef struct { u8 data[0x1B0]; } OSThread;
typedef struct {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
} OSIoMesgHdr;
typedef struct {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
} OSIoMesg;
typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)();
    s32 (*edma)();
} OSDevMgr;

extern OSDevMgr D_80078070;        /* __osViDevMgr */
extern OSThread D_800A24D8;        /* viThread */
extern u64 D_800A2690[0x1000 / 8]; /* viThreadStack */
extern OSMesgQueue D_800A3690;     /* viEventQueue */
extern OSMesg D_800A36A8[5];       /* viEventBuf */
extern OSIoMesg D_800A36C0;        /* viRetraceMsg */
extern OSIoMesg D_800A36D8;        /* viCounterMsg */

void func_800590C0(void);
void func_800585B0(OSMesgQueue *mq, OSMesg *msg, s32 count);
void func_80058CD0(u32 e, OSMesgQueue *mq, OSMesg m);
OSPri func_80062F40(OSThread *t);
void func_80058D30(OSThread *t, OSPri p);
u32 func_80062C60(void);
void func_800585E0(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, OSPri p);
void func_80063AF0(void);
void func_80058EA0(OSThread *t);
void func_80062C80(u32);
void func_80063E18(void *arg); /* viMgrMain */

void func_80063C80(OSPri pri)
{
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (D_80078070.active) {
        return;
    }
    func_800590C0();
    func_800585B0(&D_800A3690, D_800A36A8, 5);
    D_800A36C0.hdr.type = 13;
    D_800A36C0.hdr.pri = 0;
    D_800A36C0.hdr.retQueue = NULL;
    D_800A36D8.hdr.type = 14;
    D_800A36D8.hdr.pri = 0;
    D_800A36D8.hdr.retQueue = NULL;
    func_80058CD0(7, &D_800A3690, &D_800A36C0);
    func_80058CD0(3, &D_800A3690, &D_800A36D8);
    oldPri = -1;
    myPri = func_80062F40(NULL);
    if (myPri < pri) {
        oldPri = myPri;
        func_80058D30(NULL, pri);
    }
    savedMask = func_80062C60();
    D_80078070.active = 1;
    D_80078070.thread = &D_800A24D8;
    D_80078070.cmdQueue = &D_800A3690;
    D_80078070.evtQueue = &D_800A3690;
    D_80078070.acsQueue = NULL;
    D_80078070.dma = NULL;
    D_80078070.edma = NULL;
    func_800585E0(&D_800A24D8, 0, func_80063E18, &D_80078070, (u8 *)D_800A2690 + 0x1000, pri);
    func_80063AF0();
    func_80058EA0(&D_800A24D8);
    func_80062C80(savedMask);
    if (oldPri != -1) {
        func_80058D30(NULL, oldPri);
    }
}
