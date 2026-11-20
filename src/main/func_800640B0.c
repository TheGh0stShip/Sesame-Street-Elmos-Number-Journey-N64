typedef struct { u32 ctrl, width, burst, vSync, hSync, leap, hStart, xScale, vCurrent; } OSViCommonRegs;
typedef struct { u32 origin, yScale, vStart, vBurst, vIntr; } OSViFieldRegs;
typedef struct { u8 type; OSViCommonRegs comRegs; OSViFieldRegs fldRegs[2]; } OSViMode;
typedef struct { f32 factor; u16 offset; u32 scale; } __OSViScale;
typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
    OSViMode *modep;
    u32 control;
    OSMesgQueue *msgq;
    void *msg;
    __OSViScale x;
    __OSViScale y;
} __OSViContext;
extern __OSViContext *D_80078060; /* __osViCurr */
extern __OSViContext *D_80078064; /* __osViNext */
u32 func_80059460(void *); /* osVirtualToPhysical */
#define IO_READ(addr) (*(volatile u32 *)((u32)(addr) | 0xA0000000))
#define IO_WRITE(addr, data) (*(volatile u32 *)((u32)(addr) | 0xA0000000) = (u32)(data))
#define VI_BASE_REG 0x04400000
#define VI_CONTROL_REG (VI_BASE_REG + 0x00)
#define VI_ORIGIN_REG (VI_BASE_REG + 0x04)
#define VI_WIDTH_REG (VI_BASE_REG + 0x08)
#define VI_INTR_REG (VI_BASE_REG + 0x0C)
#define VI_CURRENT_REG (VI_BASE_REG + 0x10)
#define VI_BURST_REG (VI_BASE_REG + 0x14)
#define VI_V_SYNC_REG (VI_BASE_REG + 0x18)
#define VI_H_SYNC_REG (VI_BASE_REG + 0x1C)
#define VI_LEAP_REG (VI_BASE_REG + 0x20)
#define VI_H_START_REG (VI_BASE_REG + 0x24)
#define VI_V_START_REG (VI_BASE_REG + 0x28)
#define VI_V_BURST_REG (VI_BASE_REG + 0x2C)
#define VI_X_SCALE_REG (VI_BASE_REG + 0x30)
#define VI_Y_SCALE_REG (VI_BASE_REG + 0x34)
#define VI_STATE_XSCALE_UPDATED 0x02
#define VI_STATE_YSCALE_UPDATED 0x04
#define VI_STATE_BLACK 0x20
#define VI_STATE_REPEATLINE 0x40
#define VI_STATE_FADE 0x80
#define VI_SCALE_MASK 0xfff
#define VI_2_10_FPART_MASK 0x3ff
#define VI_SUBPIXEL_SH 0x10

/* __osViSwapContext: program VI registers from the next context and swap contexts */
void func_800640B0(void) {
    register OSViMode *vm;
    register __OSViContext *vc;
    u32 origin;
    u32 hStart;
    u32 nomValue;
    u32 field;

    field = 0;
    vc = D_80078064;
    vm = vc->modep;

    field = IO_READ(VI_CURRENT_REG) & 1;

    origin = func_80059460(vc->framep) + (vm->fldRegs[field].origin);
    if (vc->state & VI_STATE_XSCALE_UPDATED) {
        vc->x.scale |= (vm->comRegs.xScale & ~VI_SCALE_MASK);
    } else {
        vc->x.scale = vm->comRegs.xScale;
    }

    if (vc->state & VI_STATE_YSCALE_UPDATED) {
        nomValue = vm->fldRegs[field].yScale & VI_SCALE_MASK;
        vc->y.scale = vc->y.factor * nomValue;
        vc->y.scale |= vm->fldRegs[field].yScale & ~VI_SCALE_MASK;
    } else {
        vc->y.scale = vm->fldRegs[field].yScale;
    }

    hStart = vm->comRegs.hStart;

    if (vc->state & VI_STATE_BLACK) {
        hStart = 0;
    }

    if (vc->state & VI_STATE_REPEATLINE) {
        vc->y.scale = 0;
        origin = func_80059460(vc->framep);
    }

    if (vc->state & VI_STATE_FADE) {
        vc->y.scale = (vc->y.offset << VI_SUBPIXEL_SH) & (VI_2_10_FPART_MASK << VI_SUBPIXEL_SH);
        origin = func_80059460(vc->framep);
    }

    IO_WRITE(VI_ORIGIN_REG, origin);
    IO_WRITE(VI_WIDTH_REG, vm->comRegs.width);
    IO_WRITE(VI_BURST_REG, vm->comRegs.burst);
    IO_WRITE(VI_V_SYNC_REG, vm->comRegs.vSync);
    IO_WRITE(VI_H_SYNC_REG, vm->comRegs.hSync);
    IO_WRITE(VI_LEAP_REG, vm->comRegs.leap);
    IO_WRITE(VI_H_START_REG, hStart);
    IO_WRITE(VI_V_START_REG, vm->fldRegs[field].vStart);
    IO_WRITE(VI_V_BURST_REG, vm->fldRegs[field].vBurst);
    IO_WRITE(VI_INTR_REG, vm->fldRegs[field].vIntr);
    IO_WRITE(VI_X_SCALE_REG, vc->x.scale);
    IO_WRITE(VI_Y_SCALE_REG, vc->y.scale);
    IO_WRITE(VI_CONTROL_REG, vc->control);

    D_80078064 = D_80078060;
    D_80078060 = vc;
    *D_80078064 = *D_80078060;
}
