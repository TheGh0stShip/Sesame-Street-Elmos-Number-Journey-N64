/* SDK rmon/rmonmem.c */
#include "PR/os_version.h"

#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"


#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PRinternal/rmonint.h"
#include "PR/rdb.h"

#include "PRinternal/macros.h"

extern // TODO: this comes from a header

u8 __rmonUtilityBuffer[256];
void __rmonWriteWordTo(u32* addr, u32 val);
u32 __rmonReadWordAt(u32* addr);
void __rmonMemcpy(u8* dest, u8* srce, u32 count);
void __rmonCopyWords(u32* dest, u32* srce, u32 count);
void strcpy(char* dest, char* srce);

int __rmonReadMem(KKHeader* req) {
    char* cPtr;
    int sent;
    int dataSize;
    KKReadRequest* request = (KKReadRequest*)req;
    KKBufferEvent* reply = (KKBufferEvent*)__rmonUtilityBuffer;
    u8* blockStart;

    STUBBED_PRINTF(("ReadMem @ %08x for %d\n", request->addr, request->nbytes));

    reply->header.code = request->header.code;
    reply->object = request->object;
    reply->header.error = TV_ERROR_NO_ERROR;

    if (request->addr == (u32)-1) {
        return TV_ERROR_INVALID_ADDRESS;
    }
    if (request->nbytes > RMON_MAX_XFER_SIZE) {
        return TV_ERROR_INVALID_CAPABILITY;
    }

    if (req->method == RMON_RSP) {
        if (!((request->addr < SP_IMEM_START || (request->addr + request->nbytes) > SP_IMEM_END) ? FALSE : TRUE) &&
        !((request->addr < SP_DMEM_START || (request->addr + request->nbytes) > SP_DMEM_END) ? FALSE : TRUE)) {
                return TV_ERROR_INVALID_ADDRESS;
        }
    } else if (osVirtualToPhysical((void*)request->addr) == (u32)-1) {
        return TV_ERROR_INVALID_ADDRESS;
    }

    blockStart = (u8*)request->addr;
    reply->header.length = request->nbytes + sizeof(reply->header) + sizeof(reply->object);
    dataSize = request->nbytes + sizeof(reply->header) + sizeof(reply->object);

    cPtr = (char*)&dataSize;
    sent = 0;
    while (sent < (signed)sizeof(dataSize)) {
        sent += __osRdbSend(cPtr + sent, sizeof(dataSize) - sent, RDB_TYPE_GtoH_DEBUG);
    }

    __rmonSendHeader(&reply->header, sizeof(reply->header) + sizeof(reply->object), KK_TYPE_REPLY);
    __rmonSendData(blockStart, request->nbytes);
    return TV_ERROR_NO_ERROR;
}
int __rmonWriteMem(KKHeader* req);
int __rmonListProcesses(KKHeader* req);
int __rmonLoadProgram(KKHeader* request UNUSED);
int __rmonGetExeName(KKHeader* req);
int __rmonGetRegionCount(KKHeader* req);
int __rmonGetRegions(KKHeader* req);

