/* __osLeoAbnormalResume: reset the disk ASIC buffer manager, resume, re-enable PI interrupts */
typedef struct {
    u32 errStatus; void *dramAddr; void *C2Addr; u32 sectorSize; u32 C1ErrNum; u32 C1ErrSector[4];
} __OSBlockInfo;
typedef struct {
    u32 cmdType; u16 transferMode; u16 blockNum; s32 sectorNum; u32 devAddr;
    u32 bmCtlShadow; u32 seqCtlShadow; __OSBlockInfo block[2];
} __OSTranxInfo;
typedef struct {
    void *next; u32 baseAddress; u8 type; u8 latency; u8 pulse; u8 pageSize; u8 relDuration;
    u8 pad[3]; u32 speed; __OSTranxInfo transferInfo;
} OSPiHandle;
extern OSPiHandle *D_80106CA0; /* __osDiskHandle */
extern u32 D_80077C10;          /* __OSGlobalIntMask */
void func_80064EBC(void);       /* __osLeoResume */
#define IO_READ(a) (*(volatile u32 *)(a))
#define IO_WRITE(a, v) (*(volatile u32 *)(a) = (v))
#define WAIT_ON_IOBUSY(stat) \
    stat = IO_READ(0xA4600010); \
    while (stat & 3) stat = IO_READ(0xA4600010);

void func_80064DEC(void) {
    __OSTranxInfo *info;
    u32 pi_stat;

    info = &D_80106CA0->transferInfo;
    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(0xA5000510, info->bmCtlShadow | 0x10000000);
    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(0xA5000510, info->bmCtlShadow);
    func_80064EBC();
    IO_WRITE(0xA4600010, 2);
    D_80077C10 |= 0x00100401;
}
