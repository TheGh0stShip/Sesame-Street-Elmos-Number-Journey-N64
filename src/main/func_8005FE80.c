/* __osEPiRawStartDma: syncs PI domain timing for the handle, then starts a PI DMA */
typedef volatile u32 vu32;
typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next;
    u8 type, latency, pageSize, relDuration, pulse, domain;
    u32 baseAddress;
    u32 speed;
} OSPiHandle;
#define IO_READ(a) (*(vu32 *)((a) | 0xA0000000))
#define IO_WRITE(a, v) (*(vu32 *)((a) | 0xA0000000) = (u32)(v))
#define PI_DRAM_ADDR_REG 0x04600000
#define PI_CART_ADDR_REG 0x04600004
#define PI_RD_LEN_REG 0x04600008
#define PI_WR_LEN_REG 0x0460000C
#define PI_STATUS_REG 0x04600010
#define PI_BSD_DOM1_LAT_REG 0x04600014
#define PI_BSD_DOM1_PWD_REG 0x04600018
#define PI_BSD_DOM1_PGS_REG 0x0460001C
#define PI_BSD_DOM1_RLS_REG 0x04600020
#define PI_BSD_DOM2_LAT_REG 0x04600024
#define PI_BSD_DOM2_PWD_REG 0x04600028
#define PI_BSD_DOM2_PGS_REG 0x0460002C
#define PI_BSD_DOM2_RLS_REG 0x04600030
#define K1_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)
#define UPDATE_REG(pihandle, reg, var) \
    if (cHandle->var != pihandle->var) IO_WRITE(reg, pihandle->var)
extern OSPiHandle *D_80077F60[]; /* __osCurrentHandle */
u32 func_80059460(void *); /* osVirtualToPhysical */
s32 func_8005FE80(OSPiHandle *pihandle, s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    u32 stat;
    u32 domain;

    stat = IO_READ(PI_STATUS_REG);
    while (stat & 3)
        stat = IO_READ(PI_STATUS_REG);

    domain = pihandle->domain;
    if (D_80077F60[domain] != pihandle) {
        OSPiHandle *cHandle = D_80077F60[domain];
        if (domain == 0) {
            UPDATE_REG(pihandle, PI_BSD_DOM1_LAT_REG, latency);
            UPDATE_REG(pihandle, PI_BSD_DOM1_PGS_REG, pageSize);
            UPDATE_REG(pihandle, PI_BSD_DOM1_RLS_REG, relDuration);
            UPDATE_REG(pihandle, PI_BSD_DOM1_PWD_REG, pulse);
        } else {
            UPDATE_REG(pihandle, PI_BSD_DOM2_LAT_REG, latency);
            UPDATE_REG(pihandle, PI_BSD_DOM2_PGS_REG, pageSize);
            UPDATE_REG(pihandle, PI_BSD_DOM2_RLS_REG, relDuration);
            UPDATE_REG(pihandle, PI_BSD_DOM2_PWD_REG, pulse);
        }
        D_80077F60[domain] = pihandle;
    }
    IO_WRITE(PI_DRAM_ADDR_REG, func_80059460(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS(pihandle->baseAddress | devAddr));

    switch (direction) {
        case 0:
            IO_WRITE(PI_WR_LEN_REG, size - 1);
            break;
        case 1:
            IO_WRITE(PI_RD_LEN_REG, size - 1);
            break;
        default:
            return -1;
    }
    return 0;
}
