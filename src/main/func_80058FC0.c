/* osStopTimer */
typedef u64 OSTime;
typedef struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    OSTime interval;
    OSTime value;
    void *mq;
    void *msg;
} OSTimer;
extern OSTimer *D_80077C40; /* __osTimerList */
u32 func_80062C60(void); /* __osDisableInt */
void func_80062C80(u32); /* __osRestoreInt */
void func_80062D60(u32); /* __osSetCompare */

int func_80058FC0(OSTimer *t)
{
    register u32 savedMask;
    OSTimer *timep;

    if (t->next == NULL)
        return -1;
    savedMask = func_80062C60();
    timep = t->next;
    if (timep != D_80077C40)
        timep->value += t->value;
    t->prev->next = t->next;
    t->next->prev = t->prev;
    t->next = NULL;
    t->prev = NULL;
    if (D_80077C40->next == D_80077C40)
        func_80062D60(0);
    func_80062C80(savedMask);
    return 0;
}
