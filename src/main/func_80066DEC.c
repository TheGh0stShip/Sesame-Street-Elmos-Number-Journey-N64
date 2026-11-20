/* __rmonStopUserThreads */
typedef struct OSThread_s {
    struct OSThread_s *next; s32 priority; void *queue; struct OSThread_s *tlnext;
    u16 state; u16 flags; s32 id;
} OSThread;
OSThread *func_80068350(void);  /* __osGetActiveQueue */
void func_80068360(OSThread *t); /* osStopThread */

int func_80066DEC(int whichThread) {
    register int whichOne = 0;
    register OSThread *tptr = func_80068350();

    if (whichThread != 0) {
        while (tptr->priority != -1) {
            if (tptr->id == whichThread)
                break;
            tptr = tptr->tlnext;
        }
        if (tptr->priority == -1)
            return 0;
        if (tptr->priority > 0 && tptr->priority <= 127) {
            func_80068360(tptr);
            if (tptr->state != 1) {
            }
            whichOne = whichThread;
        }
    } else {
        while (tptr->priority != -1) {
            if (tptr->priority > 0 && tptr->priority <= 127) {
                func_80068360(tptr);
                if (tptr->state != 1) {
                }
                whichOne = -1;
            }
            tptr = tptr->tlnext;
        }
    }
    return whichOne;
}
