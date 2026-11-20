/* osCreatePiManager (libultra io/pimgr.c): start the PI device manager and ramrom threads */
typedef void *OSMesg;
typedef s32 OSPri;
typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct OSThread_s OSThread;
typedef struct OSPiHandle_s OSPiHandle;

typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(OSPiHandle *, s32, u32, void *, u32);
} OSDevMgr;

extern OSDevMgr D_80077F40;          /* __osPiDevMgr */
extern u32 D_80077F70;               /* __osPiAccessQueueEnabled */
extern OSMesgQueue D_8010E670;       /* __osPiAccessQueue */
extern OSThread D_800A07F0;          /* piThread */
extern u64 D_800A09A0[0x1000 / 8];   /* piThreadStack */
extern OSThread D_800A19A0;          /* ramromThread */
extern u64 D_800A1B50[0x400 / 8];    /* ramromThreadStack */
extern OSMesgQueue D_800A1F90;       /* piEventQueue */
extern OSMesg D_800A1FA8[];          /* piEventBuf */

void func_800585B0(OSMesgQueue *, OSMesg *, s32);                 /* osCreateMesgQueue */
void func_80060690(void);                                          /* __osPiCreateAccessQueue */
void func_80058CD0(u32, OSMesgQueue *, OSMesg);                    /* osSetEventMesg */
OSPri func_80062F40(OSThread *);                                   /* osGetThreadPri */
void func_80058D30(OSThread *, OSPri);                             /* osSetThreadPri */
u32 func_80062C60(void);                                           /* __osDisableInt */
void func_80062C80(u32);                                           /* __osRestoreInt */
void func_800585E0(OSThread *, s32, void (*)(void *), void *, void *, OSPri); /* osCreateThread */
void func_80058EA0(OSThread *);                                    /* osStartThread */
s32 func_80064550(s32, u32, void *, u32);                          /* __osPiRawStartDma */
s32 func_8005FE80(OSPiHandle *, s32, u32, void *, u32);            /* __osEPiRawStartDma */
void func_800602D0(void *);                                        /* __osDevMgrMain */
void func_8005FD9C(void *);                                        /* ramromMain */

void func_8005FBD0(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt) {
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (D_80077F40.active) {
        return;
    }
    func_800585B0(cmdQ, cmdBuf, cmdMsgCnt);
    func_800585B0(&D_800A1F90, D_800A1FA8, 1);

    if (!D_80077F70) {
        func_80060690();
    }

    func_80058CD0(8, &D_800A1F90, (OSMesg)0x22222222);
    oldPri = -1;
    myPri = func_80062F40(NULL);

    if (myPri < pri) {
        oldPri = myPri;
        func_80058D30(NULL, pri);
    }

    savedMask = func_80062C60();
    D_80077F40.active = 1;
    D_80077F40.thread = &D_800A07F0;
    D_80077F40.cmdQueue = cmdQ;
    D_80077F40.evtQueue = &D_800A1F90;
    D_80077F40.acsQueue = &D_8010E670;
    D_80077F40.dma = func_80064550;
    D_80077F40.edma = func_8005FE80;
    func_800585E0(&D_800A07F0, 0, func_800602D0, &D_80077F40, (u8 *)D_800A09A0 + sizeof(D_800A09A0), pri);
    func_80058EA0(&D_800A07F0);

    func_800585E0(&D_800A19A0, 0, func_8005FD9C, NULL, (u8 *)D_800A1B50 + sizeof(D_800A1B50), pri - 1);
    func_80058EA0(&D_800A19A0);
    func_80062C80(savedMask);

    if (oldPri != -1) {
        func_80058D30(NULL, oldPri);
    }
}
