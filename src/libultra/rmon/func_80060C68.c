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

void __rmonIOputw(u32 word) {
    int sent = 0;
    char* cPtr = (char*)&word;

    while (sent < 4) {
        sent += __osRdbSend(cPtr + sent, sizeof(word) - sent, RDB_TYPE_GtoH_DEBUG);
    }
}
void __rmonIOhandler(void);

