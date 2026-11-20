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
int __rmonGetGRegisters(KKHeader* req);
int __rmonSetGRegisters(KKHeader* req);

int __rmonGetFRegisters(KKHeader* req) {
    register KKObjectRequest* request = (KKObjectRequest*)req;
    KKFPregEvent reply;
    OSThread* tptr;
    volatile float f UNUSED;

    STUBBED_PRINTF(("GetFRegisters\n"));

    if (req->method != RMON_CPU) {
        return TV_ERROR_INVALID_ID;
    }

    /* touch fpu to ensure registers are saved to the context structure */
    f = 0.0f;

    tptr = __rmonGetTCB(request->object);
    if (tptr == NULL) {
        return TV_ERROR_INVALID_ID;
    }

    __rmonCopyWords((u32*)reply.registers.fpregs.regs, (u32*)&tptr->context.fp0, ARRLEN(reply.registers.fpregs.regs));

    reply.registers.fpcsr = tptr->context.fpcsr;
    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.tid = request->object;

    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}
int __rmonSetFRegisters(KKHeader* req);
u32 rmonGetRcpRegister(int regNumber);
int __rmonGetSRegs(KKHeader* req);
int __rmonSetSRegs(KKHeader* req);
int __rmonGetVRegs(KKHeader* req);
int __rmonSetVRegs(KKHeader* req);
u32 __rmonGetRegisterContents(int method, int threadNumber, int regNumber);

