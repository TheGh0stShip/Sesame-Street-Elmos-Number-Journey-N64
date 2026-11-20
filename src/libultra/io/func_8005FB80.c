/* SDK io/pirawread.c */
#include "PRinternal/piint.h"
#include "assert.h"
#include "PR/ultraerror.h"






































// Adjust line numbers to match assert
#line 46

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

s32 __osPiRawReadIo(u32 devAddr, u32* data) {
    register u32 stat;


    assert(data != NULL);

    WAIT_ON_IOBUSY(stat);
    *data = IO_READ((u32)osRomBase | devAddr);

    return 0;
}
