/* SDK os/timerintr.c */
#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PRinternal/osint.h"

extern OSTime __osCurrentTime;
extern u32 __osBaseCounter;
extern u32 __osViIntrCount;
extern u32 __osTimerCounter;
extern OSTimer __osBaseTimer;
extern OSTimer* __osTimerList;
extern OSMesgQueue __osProfTimerQ;
extern OSProf* __osProfileList;
extern OSProf* __osProfileListEnd;
extern u32 __osProfileOverflowBin;

void __osTimerServicesInit(void) {
    __osCurrentTime = 0;
    __osBaseCounter = 0;
    __osViIntrCount = 0;
    __osTimerList->next = __osTimerList->prev = __osTimerList;
    __osTimerList->interval = __osTimerList->value = 0;
    __osTimerList->mq = NULL;
    __osTimerList->msg = 0;
}
void __osTimerInterrupt(void);
void __osSetTimerIntr(OSTime tim);
OSTime __osInsertTimer(OSTimer* t);
