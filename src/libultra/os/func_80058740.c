#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
/* not in the SDK headers */
s32 osPiRawReadIo(s32, s32 *);
extern u64 D_80077C00;
typedef struct { u32 inst1, inst2, inst3, inst4; } __osExceptionVector;
extern __osExceptionVector D_80061DE0;
extern u8 D_8000031C[];
extern u32 D_80061DF0[];
u32 func_80062BD0(void);
extern s32 D_80077C08;
/* __osExceptionPreamble */
/* osClockRate */
 /* osViClock */
extern s32 D_800A8F80;/* __osGetSR */
/* __osSetSR */
/* __osSetFpcCsr */
/* __osSiRawReadIo */
 /* __osSiRawWriteIo */
/* osWritebackDCache */
/* osInvalICache */
 /* osMapTLBRdb */
/* osPiRawReadIo */
/* bzero */
 /* osAppNMIBuffer */

void osInitialize(void) {
    u32 pifdata;
    u32 clock = 0;

    __osSetSR(func_80062BD0() | 0x20000000);
    __osSetFpcCsr(0x01000800);
    while (__osSiRawReadIo(0x1FC007FC, &pifdata)) {
    }
    while (__osSiRawWriteIo(0x1FC007FC, pifdata | 8)) {
    }
    *(__osExceptionVector *)0x80000000 = D_80061DE0;
    *(__osExceptionVector *)0x80000080 = D_80061DE0;
    *(__osExceptionVector *)0x80000100 = D_80061DE0;
    *(__osExceptionVector *)0x80000180 = D_80061DE0;
    osWritebackDCache((void *)0x80000000, 0x190);
    osInvalICache((void *)0x80000000, 0x190);
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xF;
    if (clock != 0) {
        D_80077C00 = clock;
    }
    D_80077C00 = D_80077C00 * 3 / 4;
    if (*(s32 *)0x8000030C == 0) {
        bzero(D_8000031C, 0x40);
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
            osWritebackDCache((void *)0x80000180, 0x24);
            osInvalICache((void *)0x80000180, 0x24);
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
