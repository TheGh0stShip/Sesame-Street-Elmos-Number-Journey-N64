/* SDK io/siacs.c */
#include "PRinternal/macros.h"
#include "PR/os_internal.h"

#define SI_Q_BUF_LEN 1

extern OSMesg siAccessBuf[SI_Q_BUF_LEN];
extern OSMesgQueue __osSiAccessQueue;
extern u32 __osSiAccessQueueEnabled;

void __osSiCreateAccessQueue(void) {
    __osSiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osSiAccessQueue, siAccessBuf, SI_Q_BUF_LEN);
    osSendMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
void __osSiGetAccess(void);
void __osSiRelAccess(void);
