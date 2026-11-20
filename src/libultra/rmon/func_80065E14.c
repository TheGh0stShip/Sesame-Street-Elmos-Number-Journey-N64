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

int __rmonListProcesses(KKHeader* req) {
    KKObjectRequest* request = (KKObjectRequest*)req;
    KKObjsEvent reply;

    STUBBED_PRINTF(("ListProcesses\n"));

    reply.object = 0;
    reply.objs.number = 1;
    reply.objs.objects[0] = (req->method == RMON_RSP) ? RMON_PID_RSP : RMON_PID_CPU;
    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;

    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}
int __rmonLoadProgram(KKHeader* request UNUSED);
int __rmonGetExeName(KKHeader* req);
int __rmonGetRegionCount(KKHeader* req);
int __rmonGetRegions(KKHeader* req);

