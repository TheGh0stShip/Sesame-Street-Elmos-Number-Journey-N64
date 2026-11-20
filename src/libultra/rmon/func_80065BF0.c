#include "PR/os_version.h"
#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PRinternal/rmonint.h"
#include "PR/rdb.h"
#include "PRinternal/macros.h"

/* __rmonSendReply */
 /* __osSpRawWriteIo */
/* __osSpRawReadIo */
 /* osVirtualToPhysical */

static void __rmonWriteWordTo(u32 *addr, u32 val) {
    while (__osSpRawWriteIo((u32)addr, val) != 0) {
        ;
    }
}

static u32 __rmonReadWordAt(u32 *addr) {
    u32 data;

    if ((u32)addr >= SP_DMEM_START && (u32)addr < 0x05000000) {
        __osSpRawReadIo((u32)addr, &data);
        return data;
    }
    return 0;
}

static void __rmonMemcpy(u8 *dest, u8 *srce, u32 count) {
    while (count-- > 0) {
        *dest++ = *srce++;
    }
}

/* __rmonWriteMem: debugger memory write request handler */
int __rmonWriteMem(KKHeader *req) {
    register KKWriteRequest *request = (KKWriteRequest *)req;
    KKObjectEvent reply;

    if (req->method == RMON_CPU && osVirtualToPhysical((u32 *)request->writeHeader.addr) == (u32)-1) {
        return TV_ERROR_INVALID_ADDRESS;
    }
    if (request->writeHeader.nbytes > RMON_MAX_XFER_SIZE) {
        return TV_ERROR_INVALID_CAPABILITY;
    }

    if (((request->writeHeader.addr < SP_DMEM_START ||
         (request->writeHeader.addr + request->writeHeader.nbytes) > 0x04FFFFFF) ? FALSE : TRUE)) {
        int align;
        u32 word;

        if ((align = request->writeHeader.addr & 3) != 0) {
            if (request->writeHeader.nbytes != 1) {
                return TV_ERROR_INVALID_ADDRESS;
            }
            word = __rmonReadWordAt((u32 *)(request->writeHeader.addr & ~3));
            if (align == 1) {
                word = (word & ~0xFF0000) | (request->buffer[0] << 0x10);
            } else if (align == 2) {
                word = (word & ~0xFF00) | (request->buffer[0] << 8);
            } else {
                word = (word & ~0xFF) | (request->buffer[0] << 0);
            }
            __rmonWriteWordTo((u32 *)(request->writeHeader.addr & ~3), word);
        } else {
            int wordCount = request->writeHeader.nbytes / sizeof(u32);
            u32 *wordPointer = (u32 *)request->buffer;

            if (request->writeHeader.nbytes % sizeof(u32) != 0) {
                return TV_ERROR_INVALID_ADDRESS;
            }
            while (wordCount--) {
                __rmonWriteWordTo((u32 *)request->writeHeader.addr, *(wordPointer++));
                request->writeHeader.addr += sizeof(*wordPointer);
            }
        }
    } else {
        __rmonMemcpy((u8 *)request->writeHeader.addr, (u8 *)request->buffer, request->writeHeader.nbytes);
    }

    reply.header.code = request->writeHeader.header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.object = request->writeHeader.object;
    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);

    return TV_ERROR_NO_ERROR;
}
