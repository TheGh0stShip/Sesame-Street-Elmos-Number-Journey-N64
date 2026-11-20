/* SDK rmon/rmonsio.c */
#include "PR/os_version.h"

#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"


#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PR/ultralog.h"
#include "PR/sptask.h"
#include "PRinternal/dbgproto.h"
#include "PRinternal/rmonint.h"
#include "PR/ramrom.h"
#include "PR/rdb.h"
#include "PR/rmon.h"

#include "PRinternal/macros.h"

extern // TODO: this comes from a header

OSMesgQueue IOmq;
extern OSMesg IOmsgs;
extern void* __osRdb_DbgRead_Buf;
extern u8 rmonRdbReadBuf[RMON_DBG_BUF_SIZE];
void __rmonSendFault(OSThread* thread);
void __rmonIOflush(void);
void __rmonIOputw(u32 word);

void __rmonIOhandler(void) {
    int sent;
    char tstr[4];

    osCreateMesgQueue(&IOmq, &IOmsgs, 1);
    osSetEventMesg(OS_EVENT_RDB_DBG_DONE, &IOmq, NULL);
    __osRdb_DbgRead_Buf = rmonRdbReadBuf;

    while (TRUE) {
        osRecvMesg(&IOmq, NULL, OS_MESG_BLOCK);

        __rmonExecute((KKHeader*)&rmonRdbReadBuf);
        __osRdb_DbgRead_Buf = rmonRdbReadBuf;

        sent = 0;
        while (sent <= 0) {
            sent += __osRdbSend(tstr, 1, RDB_TYPE_GtoH_DEBUG_READY);
        }
    }
}

