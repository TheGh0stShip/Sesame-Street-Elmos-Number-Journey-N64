#include "PRinternal/macros.h"
#include "PR/os_internal.h"
/* not in the SDK headers */
extern OSMesg D_800A1FB0[1];
extern u32 D_80077F70;
          /* __osPiAccessQueueEnabled */
extern OSMesgQueue D_8010E670;   /* osRecvMesg */
static void __osPiCreateAccessQueue(void) {
    D_80077F70 = 1;
    osCreateMesgQueue(&D_8010E670, D_800A1FB0, 1);
    osSendMesg(&D_8010E670, NULL, 0);
}
void __osPiGetAccess(void) {
    OSMesg dummyMesg;
    if (!D_80077F70) {
        __osPiCreateAccessQueue();
    }
    osRecvMesg(&D_8010E670, &dummyMesg, 1);
}
