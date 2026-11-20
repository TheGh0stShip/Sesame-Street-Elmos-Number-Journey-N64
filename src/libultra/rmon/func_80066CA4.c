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
int __rmonGetFRegisters(KKHeader* req);
int __rmonSetFRegisters(KKHeader* req);
u32 rmonGetRcpRegister(int regNumber);
int __rmonGetSRegs(KKHeader* req);
int __rmonSetSRegs(KKHeader* req);
int __rmonGetVRegs(KKHeader* req);
int __rmonSetVRegs(KKHeader* req);

u32 __rmonGetRegisterContents(int method, int threadNumber, int regNumber) {
    if (method == RMON_CPU) {
        /* CPU register */
        u32* regPointer;
        OSThread* tptr;

        if (regNumber >= GREG_IDX_AT && regNumber < GREG_IDX_K0) {
            regNumber -= GREG_IDX_AT - GREG_IDX_ZERO;
        } else if (regNumber >= GREG_IDX_GP && regNumber < GREG_IDX_LO) {
            regNumber -= GREG_IDX_GP - GREG_IDX_T9;
        } else {
            return 0;
        }
        tptr = __rmonGetTCB(threadNumber);
        if (tptr == NULL) {
            return 0;
        }
        regPointer = (u32*)&tptr->context;
        regPointer += regNumber;
        return *regPointer;
    } else {
        /* RSP register */
        return rmonGetRcpRegister(regNumber);
    }
}

