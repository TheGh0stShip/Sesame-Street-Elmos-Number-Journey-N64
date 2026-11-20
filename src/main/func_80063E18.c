/* viMgrMain (vimgr.c) */
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;
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
    void *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)();
    s32 (*edma)();
} OSDevMgr;
typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
    void *modep;
    u32 control;
    OSMesgQueue *msgq;
    OSMesg msg;
} __OSViContext;

extern u16 D_800A24D0; /* retrace */
extern u32 D_800BD85C; /* __osViIntrCount */
extern u64 D_80109DE8; /* __osCurrentTime */
extern u32 D_800A7F20; /* __osBaseCounter */
__OSViContext *func_80068590(void);
s32 func_80058A70(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void func_800640B0(void);
s32 func_80058BA0(OSMesgQueue *mq, OSMesg msg, s32 flag);
u32 func_80062BC0(void);
void func_80059114(void);

void func_80063E18(void *arg)
{
    __OSViContext *vc;
    OSDevMgr *dm;
    OSIoMesg *mb;
    s32 first;
    u32 count;

    mb = NULL;
    first = 0;
    vc = func_80068590();
    D_800A24D0 = vc->retraceCount;
    if (D_800A24D0 == 0) {
        D_800A24D0 = 1;
    }
    dm = (OSDevMgr *)arg;

    while (1) {
        func_80058A70(dm->evtQueue, (OSMesg *)&mb, 1);
        switch (mb->hdr.type) {
            case 13:
                func_800640B0();
                D_800A24D0--;
                if (D_800A24D0 == 0) {
                    vc = func_80068590();
                    if (vc->msgq != NULL) {
                        func_80058BA0(vc->msgq, vc->msg, 0);
                    }
                    D_800A24D0 = vc->retraceCount;
                }
                D_800BD85C++;
                if (first) {
                    count = func_80062BC0();
                    D_80109DE8 = count;
                    first = 0;
                }
                count = D_800A7F20;
                D_800A7F20 = func_80062BC0();
                count = D_800A7F20 - count;
                D_80109DE8 = D_80109DE8 + count;
                break;
            case 14:
                func_80059114();
                break;
            default:
                break;
        }
    }
}
