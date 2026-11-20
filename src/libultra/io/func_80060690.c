/* SDK io/piacs.c */
#include "PRinternal/macros.h"
#include "PR/os_internal.h"

#define PI_Q_BUF_LEN 1

extern u32 __osPiAccessQueueEnabled;
extern OSMesg piAccessBuf[PI_Q_BUF_LEN];
extern OSMesgQueue __osPiAccessQueue;

void __osPiCreateAccessQueue(void) {
    __osPiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osPiAccessQueue, piAccessBuf, PI_Q_BUF_LEN);
    osSendMesg(&__osPiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
void __osPiGetAccess(void);
void __osPiRelAccess(void);
