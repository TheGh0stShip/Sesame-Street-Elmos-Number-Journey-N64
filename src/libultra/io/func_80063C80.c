#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PR/rcp.h"
#include "PRinternal/viint.h"
#include "PRinternal/osint.h"
/* not in the SDK headers */
extern OSMesg D_800A36A8[5];
void viMgrMain(void *arg);
extern u64 D_800A2690[0x1000 / 8];
        /* __osViDevMgr */
extern OSThread viThread;/* viThread */
 /* viThreadStack */
extern OSMesgQueue viEventQueue;/* viEventQueue */
       /* viEventBuf */
extern OSIoMesg viRetraceMsg;        /* viRetraceMsg */
extern OSIoMesg viCounterMsg;/* viCounterMsg */

OSPri func_80062F40(OSThread *t);
void osCreateThread(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, OSPri p); /* viMgrMain */

void osCreateViManager(OSPri pri)
{
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (__osViDevMgr.active) {
        return;
    }
    __osTimerServicesInit();
    osCreateMesgQueue(&viEventQueue, D_800A36A8, 5);
    viRetraceMsg.hdr.type = 13;
    viRetraceMsg.hdr.pri = 0;
    viRetraceMsg.hdr.retQueue = NULL;
    viCounterMsg.hdr.type = 14;
    viCounterMsg.hdr.pri = 0;
    viCounterMsg.hdr.retQueue = NULL;
    osSetEventMesg(7, &viEventQueue, &viRetraceMsg);
    osSetEventMesg(3, &viEventQueue, &viCounterMsg);
    oldPri = -1;
    myPri = func_80062F40(NULL);
    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(NULL, pri);
    }
    savedMask = __osDisableInt();
    __osViDevMgr.active = 1;
    __osViDevMgr.thread = &viThread;
    __osViDevMgr.cmdQueue = &viEventQueue;
    __osViDevMgr.evtQueue = &viEventQueue;
    __osViDevMgr.acsQueue = NULL;
    __osViDevMgr.dma = NULL;
    __osViDevMgr.edma = NULL;
    osCreateThread(&viThread, 0, viMgrMain, &__osViDevMgr, (u8 *)D_800A2690 + 0x1000, pri);
    __osViInit();
    osStartThread(&viThread);
    __osRestoreInt(savedMask);
    if (oldPri != -1) {
        osSetThreadPri(NULL, oldPri);
    }
}
