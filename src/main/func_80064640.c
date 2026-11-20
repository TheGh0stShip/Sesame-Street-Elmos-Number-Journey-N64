typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next;
    u8 type;
    u8 latency;
    u8 pageSize;
    u8 relDuration;
    u8 pulse;
    u8 domain;
    u32 baseAddress;
    u32 speed;
} OSPiHandle;
extern OSPiHandle *D_80077F60[]; /* __osCurrentHandle */
#define PHYS_TO_K1(x) ((u32)(x) | 0xA0000000)
#define IO_READ(addr) (*(volatile u32 *)PHYS_TO_K1(addr))
#define IO_WRITE(addr, data) (*(volatile u32 *)PHYS_TO_K1(addr) = (u32)(data))
#define PI_BASE_REG 0x04600000
#define PI_STATUS_REG (PI_BASE_REG + 0x10)
#define PI_BSD_DOM1_LAT_REG (PI_BASE_REG + 0x14)
#define PI_BSD_DOM1_PWD_REG (PI_BASE_REG + 0x18)
#define PI_BSD_DOM1_PGS_REG (PI_BASE_REG + 0x1C)
#define PI_BSD_DOM1_RLS_REG (PI_BASE_REG + 0x20)
#define PI_BSD_DOM2_LAT_REG (PI_BASE_REG + 0x24)
#define PI_BSD_DOM2_PWD_REG (PI_BASE_REG + 0x28)
#define PI_BSD_DOM2_PGS_REG (PI_BASE_REG + 0x2C)
#define PI_BSD_DOM2_RLS_REG (PI_BASE_REG + 0x30)
#define PI_STATUS_DMA_BUSY (1 << 0)
#define PI_STATUS_IO_BUSY (1 << 1)
#define PI_DOMAIN1 0
#define UPDATE_REG(pihandle, reg, var) \
    if (cHandle->var != pihandle->var) IO_WRITE(reg, pihandle->var)

/* osEPiRawReadIo: sync PI domain timing for the handle, then read a word */
s32 func_80064640(OSPiHandle *pihandle, u32 devAddr, u32 *data)
{
    register u32 stat;
    register u32 domain;

    stat = IO_READ(PI_STATUS_REG);
    while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY))
        stat = IO_READ(PI_STATUS_REG);

    domain = pihandle->domain;
    if (D_80077F60[domain] != pihandle) {
        OSPiHandle *cHandle = D_80077F60[domain];
        if (domain == PI_DOMAIN1) {
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
    *data = IO_READ(pihandle->baseAddress | devAddr);
    return 0;
}
