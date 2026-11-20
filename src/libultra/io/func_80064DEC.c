/* SDK io/leointerrupt.c */
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
#include "PRinternal/piint.h"
#include "PRinternal/macros.h"

extern OSPiHandle *__osDiskHandle;
extern u8 leoDiskStack[OS_PIM_STACKSIZE];

void __osLeoAbnormalResume(void);
void __osLeoResume(void);
s32 __osLeoInterrupt(void);

void __osLeoAbnormalResume(void) {
    __OSTranxInfo *info = &__osDiskHandle->transferInfo;
    u32 pi_stat;
    
    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_RESET);
    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow);
    __osLeoResume();
    IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
    __OSGlobalIntMask |= OS_IM_PI;
}
void __osLeoResume(void);
