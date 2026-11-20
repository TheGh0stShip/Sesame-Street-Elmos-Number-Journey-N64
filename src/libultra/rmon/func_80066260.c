/* SDK rmon/rmonregs.c */

#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PRinternal/rmonint.h"
#include "PR/rdb.h"
#include "PR/os_version.h"

#include "PRinternal/macros.h"

// TODO: these come from headers
#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"

extern u32 RCPpc;
extern u32 oldIMEMvalue;
extern u32 DMEMbuffer[4];

typedef union {
    u32 everything;
    struct {
        int opcode : 6;
        int base : 5;
        int rt : 5;
        int offset : 16;
    } scalarop;
    struct {
        int opcode : 6;
        int base : 5;
        int rt : 5;
        int size : 5;
        int element : 4;
        int offset : 7;
    } vectorop;
} INSTRUCTION;
void LoadStoreSU(int opcode, int regno);
void LoadStoreVU(int opcode, int regno);
void SetUpForRCPop(int isVector);
void CleanupFromRCPop(int isVector);

int __rmonGetGRegisters(KKHeader* req) {
    register KKObjectRequest* request = (KKObjectRequest*)req;
    KKGregEvent reply;

    STUBBED_PRINTF(("GetGRegisters\n"));

    reply.tid = request->object;
    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;

    if (request->header.method == RMON_CPU) {
        OSThread* tptr = __rmonGetTCB(request->object);
        u64* tcbregptr;
        register s32 i;

        if (tptr == NULL) {
            return TV_ERROR_INVALID_ID;
        }

        for (i = GREG_IDX_AT, tcbregptr = &tptr->context.at; i < GREG_IDX_K0; i++, tcbregptr++) {
            reply.registers.gregs[i] = *tcbregptr;
        }
        for (i = GREG_IDX_GP, tcbregptr = &tptr->context.gp; i < GREG_IDX_CAUSE; i++, tcbregptr++) {
            reply.registers.gregs[i] = *tcbregptr;
        }

        reply.registers.gregs[GREG_IDX_CAUSE] = tptr->context.cause;
        reply.registers.gregs[GREG_IDX_PC] = tptr->context.pc;
        reply.registers.gregs[GREG_IDX_SR] = tptr->context.sr;
        reply.registers.gregs[GREG_IDX_ZERO] = 0;
    } else {
        return TV_ERROR_INVALID_ID;
    }

    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}
int __rmonSetGRegisters(KKHeader* req);
int __rmonGetFRegisters(KKHeader* req);
int __rmonSetFRegisters(KKHeader* req);
u32 rmonGetRcpRegister(int regNumber);
int __rmonGetSRegs(KKHeader* req);
int __rmonSetSRegs(KKHeader* req);
int __rmonGetVRegs(KKHeader* req);
int __rmonSetVRegs(KKHeader* req);
u32 __rmonGetRegisterContents(int method, int threadNumber, int regNumber);

