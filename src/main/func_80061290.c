/* osCreateScheduler (libultra sched/sched.c) */
typedef s32 OSPri;
typedef void *OSMesg;
typedef struct { void *mtqueue; void *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct { u8 data[0x1B0]; } OSThread;
typedef struct { u8 data[0x50]; } OSViMode;
typedef struct { short type; char misc[30]; } OSScMsg;
typedef struct OSScTask_s OSScTask;
typedef struct OSScClient_s OSScClient;
typedef struct {
    OSScMsg     retraceMsg;
    OSScMsg     prenmiMsg;
    OSMesgQueue interruptQ;
    OSMesg      intBuf[8];
    OSMesgQueue cmdQ;
    OSMesg      cmdMsgBuf[8];
    OSThread    thread;
    OSScClient  *clientList;
    OSScTask    *audioListHead;
    OSScTask    *gfxListHead;
    OSScTask    *audioListTail;
    OSScTask    *gfxListTail;
    OSScTask    *curRSPTask;
    OSScTask    *curRDPTask;
    u32         frameCount;
    s32         doAudio;
} OSSched;
void func_800585B0(OSMesgQueue *mq, OSMesg *msg, s32 count);   /* osCreateMesgQueue */
void func_80063C80(OSPri pri);                                 /* osCreateViManager */
void func_80064010(OSViMode *mode);                            /* osViSetMode */
void func_8005F590(u32 active);                                /* osViBlack */
void func_80058CD0(u32 event, OSMesgQueue *mq, OSMesg msg);    /* osSetEventMesg */
void func_80063FB0(OSMesgQueue *mq, OSMesg msg, u32 retraceCount); /* osViSetEvent */
void func_800585E0(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, OSPri pri); /* osCreateThread */
void func_80058EA0(OSThread *t);                               /* osStartThread */
void func_800614C8(void *arg);                                 /* __scMain */
extern OSViMode D_80078090[];                                  /* osViModeTable */
void func_80061290(OSSched *sc, void *stack, OSPri priority, u8 mode, u8 numFields)
{
    sc->curRSPTask      = 0;
    sc->curRDPTask      = 0;
    sc->clientList      = 0;
    sc->frameCount      = 0;
    sc->audioListHead   = 0;
    sc->gfxListHead     = 0;
    sc->audioListTail   = 0;
    sc->gfxListTail     = 0;
    sc->retraceMsg.type = 1;
    sc->prenmiMsg.type  = 4;
    func_800585B0(&sc->interruptQ, sc->intBuf, 8);
    func_800585B0(&sc->cmdQ, sc->cmdMsgBuf, 8);
    func_80063C80(254);
    func_80064010(&D_80078090[mode]);
    func_8005F590(1);
    func_80058CD0(4, &sc->interruptQ, (OSMesg)0x29B);
    func_80058CD0(9, &sc->interruptQ, (OSMesg)0x29C);
    func_80058CD0(14, &sc->interruptQ, (OSMesg)0x29D);
    func_80063FB0(&sc->interruptQ, (OSMesg)0x29A, numFields);
    func_800585E0(&sc->thread, 4, func_800614C8, (void *)sc, stack, priority);
    func_80058EA0(&sc->thread);
}
