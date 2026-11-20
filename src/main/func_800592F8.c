/* __osSetTimerIntr */
typedef u64 OSTime;
extern u32 func_80062C60(void);      /* __osDisableInt */
extern void func_80062C80(u32);      /* __osRestoreInt */
extern u32 func_80062BC0(void);      /* osGetCount */
extern void func_80062D60(u32);      /* __osSetCompare */
extern u32 D_8012C404;               /* __osTimerCounter */
void func_800592F8(OSTime tim)
{
    OSTime NewTime;
    u32 savedMask;

    savedMask = func_80062C60();
    D_8012C404 = func_80062BC0();
    NewTime = D_8012C404 + tim;
    func_80062D60((u32)NewTime);
    func_80062C80(savedMask);
}
