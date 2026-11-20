/* __osInitialize_common (libultra initialize.c, with KMC debugger hook) */
typedef volatile u32 vu32;
typedef struct { u32 inst1, inst2, inst3, inst4; } __osExceptionVector;
extern __osExceptionVector D_80061DE0; /* __osExceptionPreamble */
extern u32 D_80061DF0[];
extern u64 D_80077C00; /* osClockRate */
extern s32 D_80077C08; /* osViClock */
extern s32 D_800A8F80;
u32 func_80062BD0(void); /* __osGetSR */
void func_80062D80(u32); /* __osSetSR */
void func_80062D70(u32); /* __osSetFpcCsr */
s32 func_8005F5F0(u32, u32 *); /* __osSiRawReadIo */
s32 func_8005F640(u32, u32); /* __osSiRawWriteIo */
void func_80062D90(void *, s32); /* osWritebackDCache */
void func_80062BE0(void *, s32); /* osInvalICache */
void func_80062E10(void); /* osMapTLBRdb */
s32 func_8005FB80(u32, u32 *); /* osPiRawReadIo */
void func_80060D80(void *, s32); /* bzero */
extern u8 D_8000031C[]; /* osAppNMIBuffer */

void func_80058740(void) {
    u32 pifdata;
    u32 clock = 0;

    func_80062D80(func_80062BD0() | 0x20000000);
    func_80062D70(0x01000800);
    while (func_8005F5F0(0x1FC007FC, &pifdata)) {
    }
    while (func_8005F640(0x1FC007FC, pifdata | 8)) {
    }
    *(__osExceptionVector *)0x80000000 = D_80061DE0;
    *(__osExceptionVector *)0x80000080 = D_80061DE0;
    *(__osExceptionVector *)0x80000100 = D_80061DE0;
    *(__osExceptionVector *)0x80000180 = D_80061DE0;
    func_80062D90((void *)0x80000000, 0x190);
    func_80062BE0((void *)0x80000000, 0x190);
    func_80062E10();
    func_8005FB80(4, &clock);
    clock &= ~0xF;
    if (clock != 0) {
        D_80077C00 = clock;
    }
    D_80077C00 = D_80077C00 * 3 / 4;
    if (*(s32 *)0x8000030C == 0) {
        func_80060D80(D_8000031C, 0x40);
    }
    if (*(s32 *)0x80000300 == 0) {
        D_80077C08 = 49656530;
    } else if (*(s32 *)0x80000300 == 2) {
        D_80077C08 = 48628316;
    } else {
        D_80077C08 = 48681812;
    }
    if (D_800A8F80 == 0) {
        vu32 *stat = (vu32 *)0xBFF08004;
        vu32 *base = (vu32 *)0xBFF00000;
        if (*base == 0x4B4D4300) {
            u32 *src = D_80061DF0;
            u32 entry;
            *(u32 *)0x80000180 = *src++;
            *(u32 *)0x80000184 = *src++;
            *(u32 *)0x80000188 = *src;
            src += 3;
            *(u32 *)0x80000194 = *src++;
            *(u32 *)0x80000198 = *src++;
            *(u32 *)0x8000019C = *src;
            func_80062D90((void *)0x80000180, 0x24);
            func_80062BE0((void *)0x80000180, 0x24);
            D_800A8F80 = 1;
            if (!(*stat & 0x10)) {
                entry = *(u32 *)0xBFF00004;
                if (entry != (u32)base) {
                    u32 *d = (u32 *)(entry | 0x20000000);
                    s32 n = 0x800;
                    u32 *s = (u32 *)0xBFF00000;
                    for (; n != 0; n--) {
                        *d++ = *s++;
                    }
                }
                ((void (*)(u32, u32))(entry + 8))(0x4B4D4300, 0);
            }
        }
    }
}
