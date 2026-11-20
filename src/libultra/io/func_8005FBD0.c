#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/piint.h"
#include "PR/rdb.h"
/* not in the SDK headers */
extern OSMesg D_800A1FA8[];
s32 func_80064550(s32, u32, void *, u32);
extern u64 D_800A1B50[0x400 / 8];
extern u64 D_800A09A0[0x1000 / 8];
void ramromMain(void);
       /* __osPiAccessQueue */
extern OSThread piThread;/* piThread */
   /* piThreadStack */
extern OSThread ramromThread;/* ramromThread */
    /* ramromThreadStack */
extern OSMesgQueue piEventQueue;/* piEventQueue */
          /* piEventBuf */

/* osCreateMesgQueue */
                                          /* __osPiCreateAccessQueue */
                    /* osSetEventMesg */
OSPri func_80062F40(OSThread *);/* __osEPiRawStartDma */
                                        /* __osDevMgrMain */
                                        /* ramromMain */

void osCreatePiManager(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt) {
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (__osPiDevMgr.active) {
        return;
    }
    osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
    osCreateMesgQueue(&piEventQueue, D_800A1FA8, 1);

    if (!__osPiAccessQueueEnabled) {
        __osPiCreateAccessQueue();
    }

    osSetEventMesg(8, &piEventQueue, (OSMesg)0x22222222);
    oldPri = -1;
    myPri = func_80062F40(NULL);

    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(NULL, pri);
    }

    savedMask = __osDisableInt();
    __osPiDevMgr.active = 1;
    __osPiDevMgr.thread = &piThread;
    __osPiDevMgr.cmdQueue = cmdQ;
    __osPiDevMgr.evtQueue = &piEventQueue;
    __osPiDevMgr.acsQueue = &__osPiAccessQueue;
    __osPiDevMgr.dma = func_80064550;
    __osPiDevMgr.edma = osEPiRawStartDma;
    osCreateThread(&piThread, 0, __osDevMgrMain, &__osPiDevMgr, (u8 *)D_800A09A0 + sizeof(D_800A09A0), pri);
    osStartThread(&piThread);

    osCreateThread(&ramromThread, 0, ramromMain, NULL, (u8 *)D_800A1B50 + sizeof(D_800A1B50), pri - 1);
    osStartThread(&ramromThread);
    __osRestoreInt(savedMask);

    if (oldPri != -1) {
        osSetThreadPri(NULL, oldPri);
    }
}
