/* __osLeoInterrupt (libultra io/leointerrupt.c) */
typedef volatile u32 vu32;
#define PHYS_TO_K1(x) ((u32)(x) | 0xA0000000)
#define IO_READ(addr) (*(vu32 *)PHYS_TO_K1(addr))
#define IO_WRITE(addr, data) (*(vu32 *)PHYS_TO_K1(addr) = (u32)(data))
#define PI_STATUS_REG 0x04600010
#define PI_STATUS_DMA_BUSY (1 << 0)
#define PI_STATUS_IO_BUSY (1 << 1)
#define PI_STATUS_CLR_INTR (1 << 1)
#define SR_IBIT4 0x00000800
#define OS_IM_PI 0x00100401
#define OS_READ 0
#define OS_WRITE 1
#define LEO_TRACK_MODE 2
#define LEO_SECTOR_MODE 3
#define LEO_BASE_REG 0x05000000
#define LEO_STATUS (LEO_BASE_REG + 0x508)
#define LEO_BM_CTL (LEO_BASE_REG + 0x510)
#define LEO_BM_STATUS (LEO_BASE_REG + 0x510)
#define LEO_C2_BUFF (LEO_BASE_REG + 0x000)
#define LEO_SECTOR_BUFF (LEO_BASE_REG + 0x400)
#define LEO_STATUS_DATA_REQUEST 0x40000000
#define LEO_STATUS_C2_TRANSFER 0x10000000
#define LEO_STATUS_BUFFER_MANAGER_ERROR 0x08000000
#define LEO_STATUS_MECHANIC_INTERRUPT 0x02000000
#define LEO_BM_STATUS_MICRO 0x02000000
#define LEO_BM_STATUS_C1DOUBLE 0x00400000
#define LEO_BM_STATUS_C1SINGLE 0x00200000
#define LEO_BM_CTL_CLR_MECHANIC_INTR 0x01000000
#define LEO_CMD_TYPE_0 0
#define LEO_CMD_TYPE_1 1
#define LEO_CMD_TYPE_2 2
#define LEO_ERROR_GOOD 0
#define LEO_ERROR_4 4
#define LEO_ERROR_22 22
#define LEO_ERROR_23 23
#define LEO_ERROR_24 24
#define LEO_ERROR_29 29
#define WAIT_ON_IOBUSY(stat) { stat = IO_READ(PI_STATUS_REG); while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) stat = IO_READ(PI_STATUS_REG); } (void)0

typedef struct { u32 errStatus; void *dramAddr; void *C2Addr; u32 sectorSize; u32 C1ErrNum; u32 C1ErrSector[4]; } __OSBlockInfo;
typedef struct { u32 cmdType; u16 transferMode; u16 blockNum; s32 sectorNum; u32 devAddr; u32 bmCtlShadow; u32 seqCtlShadow; __OSBlockInfo block[2]; } __OSTranxInfo;
typedef struct OSPiHandle_s { struct OSPiHandle_s *next; u8 type; u8 latency; u8 pageSize; u8 relDuration; u8 pulse; u8 domain; u32 baseAddress; u32 speed; __OSTranxInfo transferInfo; } OSPiHandle;

extern OSPiHandle *D_80106CA0; /* __osDiskHandle */
extern u32 D_80077C10; /* __OSGlobalIntMask */
s32 func_8005FE80(OSPiHandle *, s32, u32, void *, u32); /* __osEPiRawStartDma */
void func_80064DEC(void); /* __osLeoAbnormalResume */
void func_80064EBC(void); /* __osLeoResume */
#define __osDiskHandle D_80106CA0
#define __OSGlobalIntMask D_80077C10
#define __osEPiRawStartDma func_8005FE80
#define __osLeoAbnormalResume func_80064DEC
#define __osLeoResume func_80064EBC

s32 func_800648E0(void) {
    u32 stat = 0;
    volatile u32 pi_stat;
    u32 bm_stat;
    __OSTranxInfo *info = &__osDiskHandle->transferInfo;
    __OSBlockInfo *blockInfo = &info->block[info->blockNum];

    pi_stat = IO_READ(PI_STATUS_REG);
    if (pi_stat & PI_STATUS_DMA_BUSY) {
        __OSGlobalIntMask = __OSGlobalIntMask & ~SR_IBIT4; //cart interrupt
        blockInfo->errStatus = LEO_ERROR_29;
        __osLeoResume();
        return 1;
    }

    WAIT_ON_IOBUSY(pi_stat);
    stat = IO_READ(LEO_STATUS);
    if (stat & LEO_STATUS_MECHANIC_INTERRUPT) {
        WAIT_ON_IOBUSY(pi_stat);
        IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
        blockInfo->errStatus = LEO_ERROR_GOOD;
        return 0;
    }

    if (info->cmdType == LEO_CMD_TYPE_2) {
        return 1;
    }

    if (stat & LEO_STATUS_BUFFER_MANAGER_ERROR) {
        WAIT_ON_IOBUSY(pi_stat);
        stat = IO_READ(LEO_STATUS);
        blockInfo->errStatus = LEO_ERROR_22;
        __osLeoResume();
        IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
        __OSGlobalIntMask |= OS_IM_PI;
        return 1;
    }

    if (info->cmdType == LEO_CMD_TYPE_1) {
        if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
            if (info->sectorNum + 1 != info->transferMode * 85) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
            __OSGlobalIntMask |= OS_IM_PI;
            blockInfo->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
            return 1;
        } else {
            blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
            info->sectorNum++;
            __osEPiRawStartDma(__osDiskHandle, OS_WRITE, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
            return 1;
        }
    } else if (info->cmdType == LEO_CMD_TYPE_0) {
        if (info->transferMode == LEO_SECTOR_MODE) {
            if (info->sectorNum > (s32)blockInfo->C1ErrNum + 17) {
                blockInfo->errStatus = LEO_ERROR_GOOD;
                __osLeoAbnormalResume();
                return 1;
            }

            if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
                blockInfo->errStatus = LEO_ERROR_23;
                __osLeoAbnormalResume();
                return 1;
            }
        } else {
            blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
        }

        bm_stat = IO_READ(LEO_BM_STATUS);
        if ((bm_stat & LEO_BM_STATUS_C1SINGLE && bm_stat & LEO_BM_STATUS_C1DOUBLE) || bm_stat & LEO_BM_STATUS_MICRO) {
            if (blockInfo->C1ErrNum > 3) {
                if (info->transferMode != LEO_SECTOR_MODE || info->sectorNum > 0x52) {
                    blockInfo->errStatus = LEO_ERROR_23;
                    __osLeoAbnormalResume();
                    return 1;
                }
            } else {
                int errNum = blockInfo->C1ErrNum;
                blockInfo->C1ErrSector[errNum] = info->sectorNum + 1;
            }

            blockInfo->C1ErrNum++;
        }

        if (stat & LEO_STATUS_C2_TRANSFER) {
            if (info->sectorNum + 1 != 88) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
            }

            if (info->transferMode == LEO_TRACK_MODE && info->blockNum == 0) {
                info->blockNum = 1;
                info->sectorNum = -1;
                info->block[1].dramAddr = (void *)((u32)info->block[1].dramAddr - info->block[1].sectorSize);

                blockInfo->errStatus = LEO_ERROR_22;
            } else {
                IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
                __OSGlobalIntMask |= OS_IM_PI;
                info->cmdType = LEO_CMD_TYPE_2;
                blockInfo->errStatus = LEO_ERROR_GOOD;
            }

            __osEPiRawStartDma(__osDiskHandle, OS_READ, LEO_C2_BUFF, blockInfo->C2Addr, blockInfo->sectorSize * 4);
            return 1;
        }

        if (info->sectorNum == -1 && info->transferMode == LEO_TRACK_MODE && info->blockNum == 1) {
            __OSBlockInfo *bptr = &info->block[0];
            if (bptr->C1ErrNum == 0) {
                if (((u32 *)bptr->C2Addr)[0] | ((u32 *)bptr->C2Addr)[1] | ((u32 *)bptr->C2Addr)[2] | ((u32 *)bptr->C2Addr)[3]) {
                    bptr->errStatus = LEO_ERROR_24;
                    __osLeoAbnormalResume();
                    return 1;
                }
            }

            bptr->errStatus = 0;
            __osLeoResume();
        }
        info->sectorNum++;
        if (stat & LEO_STATUS_DATA_REQUEST) {
            if (info->sectorNum > 0x54) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            __osEPiRawStartDma(__osDiskHandle, 0, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
            blockInfo->errStatus = LEO_ERROR_GOOD;
            return 1;
        } else if (info->sectorNum <= 0x54) {
            blockInfo->errStatus = LEO_ERROR_24;
            __osLeoAbnormalResume();
            return 1;
        }

        return 1;
    } else {
        blockInfo->errStatus = LEO_ERROR_4;
        __osLeoAbnormalResume();
        return 1;
    }
}

