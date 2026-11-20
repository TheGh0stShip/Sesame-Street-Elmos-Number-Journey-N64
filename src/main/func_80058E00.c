/* osSetTimer */
typedef u64 OSTime;
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    OSTime interval;
    OSTime value;
    OSMesgQueue *mq;
    OSMesg msg;
} OSTimer;
extern OSTimer *D_80077C40; /* __osTimerList */
OSTime func_80059358(OSTimer *t); /* __osInsertTimer */
void func_800592F8(OSTime t); /* __osSetTimerIntr */

int func_80058E00(OSTimer *t, OSTime value, OSTime interval, OSMesgQueue *mq, OSMesg msg)
{
    OSTime time;
    t->next = NULL;
    t->prev = NULL;
    t->interval = interval;
    t->value = value;
    if (value == 0)
        t->value = interval;
    t->mq = mq;
    t->msg = msg;
    time = func_80059358(t);
    if (D_80077C40->next == t)
        func_800592F8(time);
    return 0;
}
