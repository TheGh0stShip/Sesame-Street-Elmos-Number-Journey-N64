/* Set or clear bit 0x20 of the current VI context's control halfword, interrupts off. */
typedef struct { u16 unk_00; } ViCtx;
extern ViCtx *D_80078064;
u32 func_80062C60(void);
void func_80062C80(u32);
void func_8005F590(u8 on) {
    u32 saveMask = func_80062C60();
    if (on) {
        D_80078064->unk_00 |= 0x20;
    } else {
        D_80078064->unk_00 &= ~0x20;
    }
    func_80062C80(saveMask);
}
