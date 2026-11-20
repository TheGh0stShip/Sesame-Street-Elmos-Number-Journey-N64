/* osViSetSpecialFeatures */
typedef struct { u8 type; u32 ctrl; } OSViMode; /* comRegs.ctrl at 0x4 */
typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
    OSViMode *modep;
    u32 control;
} __OSViContext;
extern __OSViContext *D_80078064; /* __osViNext */
u32 func_80062C60(void); /* __osDisableInt */
void func_80062C80(u32); /* __osRestoreInt */

void func_8005F420(u32 func)
{
    register u32 saveMask = func_80062C60();

    if (func & 1) D_80078064->control |= 8;
    if (func & 2) D_80078064->control &= ~8;
    if (func & 4) D_80078064->control |= 4;
    if (func & 8) D_80078064->control &= ~4;
    if (func & 0x10) D_80078064->control |= 0x10;
    if (func & 0x20) D_80078064->control &= ~0x10;
    if (func & 0x40) {
        D_80078064->control |= 0x10000;
        D_80078064->control &= ~0x300;
    }
    if (func & 0x80) {
        D_80078064->control &= ~0x10000;
        D_80078064->control |= D_80078064->modep->ctrl & 0x300;
    }
    D_80078064->state |= 8;
    func_80062C80(saveMask);
}
