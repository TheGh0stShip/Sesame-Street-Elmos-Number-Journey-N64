/* reads a field of the running thread with interrupts disabled */
typedef struct { struct OSThread_s *next; s32 priority; } OSThread;
extern u32 func_80062C60(void);        /* __osDisableInt */
extern void func_80062C80(u32);        /* __osRestoreInt */
extern OSThread *D_80078060;           /* __osRunningThread */
s32 func_80063C00(void)
{
    u32 mask = func_80062C60();
    s32 pri = D_80078060->priority;
    func_80062C80(mask);
    return pri;
}
