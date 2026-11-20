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
int __rmonReadMem(KKHeader* req);
int __rmonWriteMem(KKHeader* req);
int __rmonListProcesses(KKHeader* req);
int __rmonLoadProgram(KKHeader* request UNUSED);
int __rmonGetExeName(KKHeader* req);
int __rmonGetRegionCount(KKHeader* req);

int __rmonGetRegions(KKHeader* req) {
    KKObjectRequest* request = (KKObjectRequest*)req;
    KKRegionEvent* reply = (KKRegionEvent*)__rmonUtilityBuffer;
    int numRegions;

    STUBBED_PRINTF(("GetRegions\n"));

    numRegions = (req->method == RMON_RSP) ? 2 : 6;

    reply->header.length = numRegions * sizeof(reply->regions[0]) + sizeof(*reply);
    reply->header.code = request->header.code;
    reply->header.error = TV_ERROR_NO_ERROR;
    reply->object = request->object;
    reply->number = numRegions;

    reply->regions[1].vaddr = SP_IMEM_START;
    reply->regions[1].size = SP_IMEM_END + 1 - SP_IMEM_START;
    reply->regions[1].flags = 1 | 2 | 4;
    reply->regions[1].paddr = SP_IMEM_START;

    reply->regions[0].vaddr = SP_DMEM_START;
    reply->regions[0].size = SP_DMEM_END + 1 - SP_DMEM_START;
    reply->regions[0].flags = 1 | 2;
    reply->regions[0].paddr = SP_DMEM_START;

    if (numRegions > 2) {
        reply->regions[2].vaddr = 0x88200000;
        reply->regions[2].size = 0x6130;
        reply->regions[2].flags = 1 | 4;
        reply->regions[2].paddr = 0;

        reply->regions[3].vaddr = 4;
        reply->regions[3].size = 0x200000;
        reply->regions[3].flags = 1 | 2;
        reply->regions[3].paddr = 0;

        reply->regions[4].vaddr = 0x4002000;
        reply->regions[4].size = 0x800000;
        reply->regions[4].flags = 1 | 2;
        reply->regions[4].paddr = 0;

        reply->regions[5].vaddr = 0x88206130;
        reply->regions[5].size = 0x9000;
        reply->regions[5].flags = 1 | 2;
        reply->regions[5].paddr = 0;
    }

    __rmonSendReply(&reply->header, reply->header.length, KK_TYPE_REPLY);

    return TV_ERROR_NO_ERROR;
}

