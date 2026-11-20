/* __osInsertTimer */
typedef struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    u64 interval;
    u64 value;
    void *mq;
    void *msg;
} OSTimer;
extern OSTimer *D_80077C40; /* __osTimerList */
u32 func_80062C60(void); /* __osDisableInt */
void func_80062C80(u32); /* __osRestoreInt */

u64 func_80059358(OSTimer *t)
{
    OSTimer *timep;
    u64 tim;
    u32 savedMask;

    savedMask = func_80062C60();
    for (timep = D_80077C40->next, tim = t->value;
         timep != D_80077C40 && tim > timep->value;
         tim -= timep->value, timep = timep->next) {
        ;
    }
    t->value = tim;
    if (timep != D_80077C40)
        timep->value -= tim;
    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    func_80062C80(savedMask);
    return tim;
}
