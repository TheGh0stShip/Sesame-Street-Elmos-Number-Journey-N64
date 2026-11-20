/* __osViInit */
typedef struct { f32 factor; u16 offset; u32 scale; } __OSViScale;
typedef struct { u32 ctrl; u32 width; u32 burst; u32 vSync; u32 hSync; u32 leap; u32 hStart; u32 xScale; u32 vCurrent; } OSViCommonRegs;
typedef struct { u32 origin; u32 yScale; u32 vStart; u32 vBurst; u32 vIntr; } OSViFieldRegs;
typedef struct { u8 type; OSViCommonRegs comRegs; OSViFieldRegs fldRegs[2]; } OSViMode;
typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
    OSViMode *modep;
    u32 control;
    void *msgq;
    void *msg;
    __OSViScale x;
    __OSViScale y;
} __OSViContext;

extern __OSViContext D_80078000[2]; /* vi */
extern __OSViContext *D_80078060;   /* __osViCurr */
extern __OSViContext *D_80078064;   /* __osViNext */
extern s32 D_80000300;              /* osTvType */
extern OSViMode D_80078E10, D_80078E60, D_80078DC0;
void func_80060D80(void *, s32);    /* bzero */
void func_800640B0(void);           /* __osViSwapContext */

#define IO_READ(a) (*(volatile u32 *)((a) | 0xA0000000))
#define IO_WRITE(a, v) (*(volatile u32 *)((a) | 0xA0000000) = (v))

void func_80063AF0(void)
{
    func_80060D80(D_80078000, sizeof(D_80078000));
    D_80078060 = &D_80078000[0];
    D_80078064 = &D_80078000[1];
    D_80078064->retraceCount = 1;
    D_80078060->retraceCount = 1;
    D_80078064->framep = (void *)0x80000000;
    D_80078060->framep = (void *)0x80000000;
    if (D_80000300 == 0) {
        D_80078064->modep = &D_80078E10;
    } else if (D_80000300 == 2) {
        D_80078064->modep = &D_80078E60;
    } else {
        D_80078064->modep = &D_80078DC0;
    }
    D_80078064->state = 0x20;
    D_80078064->control = D_80078064->modep->comRegs.ctrl;
    while (IO_READ(0x04400010) > 10)
        ;
    IO_WRITE(0x04400000, 0);
    func_800640B0();
}
