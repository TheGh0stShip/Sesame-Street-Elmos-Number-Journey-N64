typedef struct { u8 pad[0x10]; u16 state; } OSThread;
extern OSThread *D_80077C30; /* __osRunningThread */
extern OSThread *D_80077C28; /* __osRunQueue */
s32 func_80062C60(void);
void func_80062C80(s32);
void func_800628CC(OSThread **);
/* osYieldThread */
void func_800633F0(void) {
    s32 mask = func_80062C60();
    D_80077C30->state = 2;
    func_800628CC(&D_80077C28);
    func_80062C80(mask);
}
