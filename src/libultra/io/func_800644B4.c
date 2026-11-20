#include "PRinternal/macros.h"
#include "PR/os_internal.h"
/* not in the SDK headers */
extern s32 D_80078DB0;
extern OSMesg D_800A36F0[1];
          /* __osPiAccessQueueEnabled */
extern OSMesgQueue D_8013EF10;   /* osRecvMesg */
static void __osPiCreateAccessQueue(void) {
    D_80078DB0 = 1;
    osCreateMesgQueue(&D_8013EF10, D_800A36F0, 1);
    osSendMesg(&D_8013EF10, NULL, 0);
}
void __osSiGetAccess(void) {
    OSMesg dummyMesg;
    if (!D_80078DB0) {
        __osPiCreateAccessQueue();
    }
    osRecvMesg(&D_8013EF10, &dummyMesg, 1);
}
