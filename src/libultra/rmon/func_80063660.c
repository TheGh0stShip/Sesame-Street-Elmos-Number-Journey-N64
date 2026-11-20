/* SDK host/readhost.c */

#include "PR/os_internal.h"
#include "PR/rdb.h"

#include "PRinternal/macros.h"

extern int readHostInitialized;
extern OSMesgQueue readHostMesgQueue;
extern OSMesg readHostMesgBuf[1];
extern u32 __osRdb_Read_Data_Buf;
extern u32 __osRdb_Read_Data_Ct;

void osReadHost(void* dramAddr, u32 nbytes) {
    char tstr[4];
    u32 sent = 0;

    if (!readHostInitialized) {
        osCreateMesgQueue(&readHostMesgQueue, readHostMesgBuf, ARRLEN(readHostMesgBuf));
        osSetEventMesg(OS_EVENT_RDB_READ_DONE, &readHostMesgQueue, NULL);
        readHostInitialized = TRUE;
    }

    __osRdb_Read_Data_Buf = dramAddr;
    __osRdb_Read_Data_Ct = nbytes;

    while (sent == 0) {
        sent += __osRdbSend(tstr, 1, RDB_TYPE_GtoH_READY_FOR_DATA);
    }

    osRecvMesg(&readHostMesgQueue, NULL, OS_MESG_BLOCK);
    return;
}

