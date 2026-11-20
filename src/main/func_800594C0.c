/* arena init under disabled interrupts: store base and word count, clear three counters */
u32 func_80062C60(void);
void func_80062C80(u32 m);
extern u32 D_8013C728;
extern u32 D_80107D18;
extern s32 D_8013ECF4;
extern s32 D_800AD008;
extern s32 D_80128574;
void func_800594C0(u32 base, u32 size)
{
    u32 m;
    size >>= 2;
    if (base & 3) {
        base = (base & 3) + 4;
        size--;
    }
    m = func_80062C60();
    D_8013C728 = base;
    D_80107D18 = size;
    D_8013ECF4 = 0;
    D_800AD008 = 0;
    D_80128574 = 0;
    func_80062C80(m);
}
