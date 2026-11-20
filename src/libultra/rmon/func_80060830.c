/* SDK rmon/rmonmain.c */
#include "PR/os_version.h"

#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"


#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PRinternal/rmonint.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PR/rdb.h"

#include "PRinternal/macros.h"

extern // TODO: this comes from a header

int __rmonActive;
extern vu32 somethingToDo;
extern u32 inbuffer[280];
extern u8 cmdinptr;
extern u8 cmdoutptr;
extern int state;
extern char* inPointer;

void __rmonSendHeader(KKHeader* const block, u32 blockSize, u32 type) {
    int sent;
    char* cPtr = (char*)block;

    block->rev = KK_REV;
    block->type = type;

    sent = 0;
    while (sent < blockSize) {
        sent += __osRdbSend(cPtr + sent, blockSize - sent, RDB_TYPE_GtoH_DEBUG);
    }
}
void __rmonSendReply(KKHeader* const block, u32 blockSize, u32 replyType);
void __rmonSendData(char* const block, unsigned int blockSize);
void rmonMain(void);

