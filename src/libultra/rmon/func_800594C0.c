/* SDK os/initrdb.c */
#include "PR/rdb.h"
#include "PR/os_internal.h"

extern rdbPacket* __osRdb_IP6_Data;
extern u32 __osRdb_IP6_Size;
extern u32 __osRdb_IP6_Ct;
extern u32 __osRdb_IP6_CurWrite;
extern u32 __osRdb_IP6_CurSend;

void osInitRdb(u8* sendBuf, u32 sendSize) {
    u32 mask;

    sendSize /= 4;

    if (((u32)sendBuf & 3) != 0) {
        sendBuf = ((u32)sendBuf & 3) + 4;
        sendSize--;
    }
    mask = __osDisableInt();

    __osRdb_IP6_Data = sendBuf;
    __osRdb_IP6_Size = sendSize;
    __osRdb_IP6_Ct = 0;
    __osRdb_IP6_CurWrite = 0;
    __osRdb_IP6_CurSend = 0;

    __osRestoreInt(mask);
}

