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
void __osTimerServicesInit(void);
void __osTimerInterrupt(void);

void __osSetTimerIntr(OSTime tim) {
    OSTime NewTime;
    u32 savedMask;


    savedMask = __osDisableInt();
    __osTimerCounter = osGetCount();
    NewTime = __osTimerCounter + tim;
    __osSetCompare(NewTime);
    __osRestoreInt(savedMask);
}
OSTime __osInsertTimer(OSTimer* t);
