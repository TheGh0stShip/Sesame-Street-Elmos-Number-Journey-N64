/* with interrupts disabled, store a value at +4 of *D_80078064 and set flag 0x10 */
u32 func_80062C60(void);
void func_80062C80(u32);
typedef struct { u16 flags; u16 pad2; s32 unk_4; } S;
extern S *D_80078064;
void func_80064060(s32 v)
{
    u32 saveMask = func_80062C60();
    D_80078064->unk_4 = v;
    D_80078064->flags |= 0x10;
    func_80062C80(saveMask);
}
