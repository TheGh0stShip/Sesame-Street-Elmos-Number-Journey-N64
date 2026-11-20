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
void __osSetTimerIntr(OSTime tim);

OSTime __osInsertTimer(OSTimer* t) {
    OSTimer* timep;
    OSTime tim;
    u32 savedMask = __osDisableInt();

    timep = __osTimerList->next;
    tim = t->value;
    for (; timep != __osTimerList && tim > timep->value; timep = timep->next) {
        tim -= timep->value;
    }

    t->value = tim;

    if (timep != __osTimerList) {
        timep->value -= tim;
    }

    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    __osRestoreInt(savedMask);
    return tim;
}
