/* with interrupts disabled, OR a mask into D_80077C10 */
u32 func_80062C60(void);   /* __osDisableInt */
void func_80062C80(u32);   /* __osRestoreInt */
extern u32 D_80077C10;
void func_800633B0(u32 mask)
{
    u32 saveMask = func_80062C60();
    D_80077C10 |= mask;
    func_80062C80(saveMask);
}
