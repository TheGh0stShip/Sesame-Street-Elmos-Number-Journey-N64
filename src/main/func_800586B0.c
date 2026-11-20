extern u32 D_800A7F20; /* __osBaseCounter */
extern u64 D_80109DE8; /* __osCurrentTime */
s32 func_80062C60(void);
void func_80062C80(s32);
u32 func_80062BC0(void); /* osGetCount */
/* osGetTime */
u64 func_800586B0(void) {
    u32 tmptime;
    u32 elapseCount;
    u64 currentCount;
    register u32 saveMask;
    saveMask = func_80062C60();
    tmptime = func_80062BC0();
    elapseCount = tmptime - D_800A7F20;
    currentCount = D_80109DE8;
    func_80062C80(saveMask);
    return currentCount + elapseCount;
}
