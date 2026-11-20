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

void __osTimerInterrupt(void) {
    OSTimer* t;
    u32 count;
    u32 elapsed_cycles;

    u32 pc;
    s32 offset;
    OSProf* prof = __osProfileList;

    if (__osTimerList->next == __osTimerList) {
        return;
    }
    for (;;) {
        t = __osTimerList->next;

        if (t == __osTimerList) {
            __osSetCompare(0);
            __osTimerCounter = 0;
            break;
        }

        count = osGetCount();
        elapsed_cycles = count - __osTimerCounter;
        __osTimerCounter = count;

        if (elapsed_cycles < t->value) {
            t->value -= elapsed_cycles;
            __osSetTimerIntr(t->value);
            break;
        }

        t->prev->next = t->next;
        t->next->prev = t->prev;
        t->next = NULL;
        t->prev = NULL;

        if (t->mq != NULL) {
            if (t->mq != &__osProfTimerQ) {
                osSendMesg(t->mq, t->msg, OS_MESG_NOBLOCK);
            } else {
                pc = __osRunQueue->context.pc;
                for (prof = __osProfileList; prof < __osProfileListEnd; prof++) {
                    offset = pc - (u32)prof->text_start;

                    if (offset >= 0) {
                        if ((s32)prof->text_end - (s32)pc > 0) {
                            (*(u16*)(u32)((offset >> 2) + prof->histo_base))++;
                            goto __ProfDone;
                        }
                    }
                }

                __osProfileOverflowBin++;
            }
        }

    __ProfDone:

        if (t->interval != 0) {
            t->value = t->interval;
            __osInsertTimer(t);
        }
    }
}
void __osSetTimerIntr(OSTime tim);
OSTime __osInsertTimer(OSTimer* t);
