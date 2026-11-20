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

void __rmonSendFault(OSThread* thread) {
    volatile float f UNUSED;
    u8* tPtr;
    u32 sent = 0;

    /* touch fpu to ensure registers are saved to the context structure */
    f = 0.0f;

    tPtr = (u8*)thread;
    while (sent < sizeof(OSThread)) {
        sent += __osRdbSend(tPtr + sent, sizeof(OSThread) - sent, RDB_TYPE_GtoH_FAULT);
    }
}
void __rmonIOflush(void);
void __rmonIOputw(u32 word);
void __rmonIOhandler(void);

