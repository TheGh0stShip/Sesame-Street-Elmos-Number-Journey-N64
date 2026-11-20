/* SDK io/cartrominit.c */
#include "PRinternal/macros.h"
#include "PR/os_version.h"
#include "PR/os_internal.h"
#include "PR/R4300.h"
#include "PR/rcp.h"
#include "PRinternal/piint.h"

extern OSPiHandle CartRomHandle;
OSPiHandle* osCartRomInit(void) {
    u32 domain = 0;
    u32 saveMask;

    if (CartRomHandle.baseAddress == PHYS_TO_K1(PI_DOM1_ADDR2))
        return &CartRomHandle;

    CartRomHandle.type = DEVICE_TYPE_CART;
    CartRomHandle.baseAddress = PHYS_TO_K1(PI_DOM1_ADDR2);
    osPiRawReadIo(NULL, &domain);
    CartRomHandle.latency = domain & 0xff;
    CartRomHandle.pulse = (domain >> 8) & 0xff;
    CartRomHandle.pageSize = (domain >> 0x10) & 0xf;
    CartRomHandle.relDuration = (domain >> 0x14) & 0xf;
    CartRomHandle.domain = PI_DOMAIN1;
    CartRomHandle.speed = 0;

    bzero(&CartRomHandle.transferInfo, sizeof(__OSTranxInfo));

    saveMask = __osDisableInt();
    CartRomHandle.next = __osPiTable;
    __osPiTable = &CartRomHandle;
    __osRestoreInt(saveMask);

    return &CartRomHandle;
}
