#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/piint.h"
#include "PR/rdb.h"
/* not in the SDK headers */
extern char D_800A1F68[];
void func_80060750(void);
extern char D_800A1F88[];
extern char D_800A1F50[];
extern char D_800A1F70[];

void ramromMain(void)
{
    u8 buf[8];
    s32 n;

    osCreateMesgQueue(D_800A1F50, D_800A1F68, 1);
    osCreateMesgQueue(D_800A1F70, D_800A1F88, 1);
    osSetEventMesg(0x12, D_800A1F50, 0);
    osSetEventMesg(0x13, D_800A1F70, 0);
    for (;;) {
        osRecvMesg(D_800A1F50, 0, 1);
        __osPiGetAccess();
        n = 0;
        do {
            n += __osRdbSend(buf, 1, 9);
        } while (n == 0);
        osRecvMesg(D_800A1F70, 0, 1);
        func_80060750();
    }
}
