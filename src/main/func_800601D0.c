/* osLeoDiskInit */
typedef struct { u8 data[0x60]; } __OSTranxInfo;
typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next;
    u8 type, latency, pageSize, relDuration, pulse, domain;
    u32 baseAddress;
    u32 speed;
    __OSTranxInfo transferInfo;
} OSPiHandle;
extern OSPiHandle D_8012F168;    /* LeoDiskHandle */
extern OSPiHandle *D_80106CA0;   /* __osDiskHandle */
extern OSPiHandle *D_80077F5C;   /* __osPiTable */
void func_80060D80(void *p, s32 len);        /* bzero */
u32 func_80062C60(void);                     /* __osDisableInt */
void func_80062C80(u32 mask);                /* __osRestoreInt */

#define IO_WRITE(a, v) (*(volatile u32 *)(a) = (v))

OSPiHandle *func_800601D0(void) {
    u32 saveMask;

    D_8012F168.type = 2;
    D_8012F168.baseAddress = 0xA5000000;
    D_8012F168.latency = 3;
    D_8012F168.pulse = 6;
    D_8012F168.pageSize = 6;
    D_8012F168.relDuration = 2;
    D_8012F168.domain = 1;

    IO_WRITE(0xA4600024, D_8012F168.latency);
    IO_WRITE(0xA4600028, D_8012F168.pulse);
    IO_WRITE(0xA460002C, D_8012F168.pageSize);
    IO_WRITE(0xA4600030, D_8012F168.relDuration);
    D_8012F168.speed = 0;

    func_80060D80(&D_8012F168.transferInfo, sizeof(__OSTranxInfo));

    saveMask = func_80062C60();
    D_8012F168.next = D_80077F5C;
    D_80077F5C = &D_8012F168;
    D_80106CA0 = &D_8012F168;
    func_80062C80(saveMask);

    return &D_8012F168;
}
