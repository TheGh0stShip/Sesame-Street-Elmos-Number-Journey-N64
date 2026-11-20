/* osCartRomInit (pre-2.0J form) */
typedef struct { u8 data[0x60]; } __OSTranxInfo;
typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next;
    u8 type, latency, pageSize, relDuration, pulse, domain;
    u32 baseAddress;
    u32 speed;
    __OSTranxInfo transferInfo;
} OSPiHandle;
extern OSPiHandle D_8013EC08;    /* CartRomHandle */
extern OSPiHandle *D_80077F5C;   /* __osPiTable */
s32 func_8005FB80(u32 devAddr, u32 *data);   /* osPiRawReadIo */
void func_80060D80(void *p, s32 len);        /* bzero */
u32 func_80062C60(void);                     /* __osDisableInt */
void func_80062C80(u32 mask);                /* __osRestoreInt */

OSPiHandle *func_800600F0(void) {
    u32 domain = 0;
    u32 saveMask;

    if (D_8013EC08.baseAddress == 0xB0000000)
        return &D_8013EC08;

    D_8013EC08.type = 0;
    D_8013EC08.baseAddress = 0xB0000000;
    func_8005FB80(0, &domain);
    D_8013EC08.latency = domain & 0xff;
    D_8013EC08.pulse = (domain >> 8) & 0xff;
    D_8013EC08.pageSize = (domain >> 0x10) & 0xf;
    D_8013EC08.relDuration = (domain >> 0x14) & 0xf;
    D_8013EC08.domain = 0;
    D_8013EC08.speed = 0;

    func_80060D80(&D_8013EC08.transferInfo, sizeof(__OSTranxInfo));

    saveMask = func_80062C60();
    D_8013EC08.next = D_80077F5C;
    D_80077F5C = &D_8013EC08;
    func_80062C80(saveMask);

    return &D_8013EC08;
}
