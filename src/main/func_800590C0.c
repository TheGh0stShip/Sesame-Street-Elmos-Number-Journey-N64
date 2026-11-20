typedef struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    u64 interval;
    u64 remaining;
    void *mq;
    void *msg;
} OSTimer;
extern u64 D_80109DE8;   /* __osCurrentTime */
extern u32 D_800A7F20;   /* __osBaseCounter */
extern u32 D_800BD85C;   /* __osViIntrCount */
extern OSTimer *D_80077C40; /* __osTimerList */
/* __osTimerServicesInit */
void func_800590C0(void) {
    D_80109DE8 = 0;
    D_800A7F20 = 0;
    D_800BD85C = 0;
    D_80077C40->prev = D_80077C40;
    D_80077C40->next = D_80077C40->prev;
    D_80077C40->remaining = D_80109DE8;
    D_80077C40->interval = D_80077C40->remaining;
    D_80077C40->mq = NULL;
    D_80077C40->msg = 0;
}
