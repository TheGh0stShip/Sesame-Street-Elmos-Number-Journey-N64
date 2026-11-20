/* finds the idle (priority 0) thread on the active queue and sets its FPU control/status bits */
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u8 pad[0x118 - 0x10];
    u32 fpcsr;
} OSThread;
OSThread *func_80068350(void); /* __osGetActiveQueue */
void func_80066D20(void) {
    OSThread *t = func_80068350();
    while (t->priority != -1) {
        if (t->priority == 0) {
            t->fpcsr = (t->fpcsr & 0xFFFF00FE) | 0x6C01;
            break;
        }
        t = t->tlnext;
    }
}
