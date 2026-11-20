#include "PR/os_version.h"
#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PRinternal/rmonint.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PR/rdb.h"
#include "PRinternal/macros.h"
/* __osRdbSend */
 /* __rmonIOflush */

static inline void __rmonSendHeader(KKHeader *const block, u32 blockSize, u32 type)
{
    int sent;
    char *cPtr = (char *)block;

    block->rev = 2;
    block->type = type;
    sent = 0;
    while (sent < blockSize)
        sent += __osRdbSend(cPtr + sent, blockSize - sent, 8);
}

void __rmonSendReply(KKHeader *const block, u32 blockSize, u32 replyType)
{
    char *cPtr;
    int sent = 0;

    block->length = blockSize;
    cPtr = (char *)&blockSize;
    while (sent < (int)sizeof(blockSize))
        sent += __osRdbSend(cPtr + sent, sizeof(blockSize) - sent, 8);
    __rmonSendHeader(block, blockSize, replyType);
    __rmonIOflush();
}
