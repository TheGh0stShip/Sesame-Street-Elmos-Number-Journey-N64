/* osEPiRawWriteIo: sync PI domain timing for the handle, then write one word to the device */
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

extern OSPiHandle *D_80077F60[2]; /* __osCurrentHandle */

#define IO_READ(a) (*(volatile u32 *)(0xA0000000 | (a)))
#define IO_WRITE(a, d) (*(volatile u32 *)(0xA0000000 | (a)) = (d))
#define PI_STATUS_REG 0x04600010
#define UPDATE_REG(pihandle, reg, var) \
    if (cHandle->var != pihandle->var) \
        IO_WRITE(reg, pihandle->var)

s32 func_80064790(OSPiHandle *pihandle, u32 devAddr, u32 data)
{
    register u32 stat;
    register u32 domain;

    stat = IO_READ(PI_STATUS_REG);
    while (stat & 3)
        stat = IO_READ(PI_STATUS_REG);

    domain = pihandle->domain;
    if (D_80077F60[domain] != pihandle) {
        OSPiHandle *cHandle = D_80077F60[domain];
        if (domain == 0) {
            UPDATE_REG(pihandle, 0x04600014, latency);
            UPDATE_REG(pihandle, 0x0460001C, pageSize);
            UPDATE_REG(pihandle, 0x04600020, relDuration);
            UPDATE_REG(pihandle, 0x04600018, pulse);
        } else {
            UPDATE_REG(pihandle, 0x04600024, latency);
            UPDATE_REG(pihandle, 0x0460002C, pageSize);
            UPDATE_REG(pihandle, 0x04600030, relDuration);
            UPDATE_REG(pihandle, 0x04600028, pulse);
        }
        D_80077F60[domain] = pihandle;
    }
    IO_WRITE(pihandle->baseAddress | devAddr, data);
    return 0;
}
