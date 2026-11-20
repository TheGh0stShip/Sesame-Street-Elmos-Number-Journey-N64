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

static void LoadStoreSU(int opcode, int regno) {
    INSTRUCTION inst;

    /* Prepare a scalar load or store instruction at DMEM address 0 */
    inst.everything = 0;
    inst.scalarop.opcode = opcode;
    inst.scalarop.rt = regno;
    __rmonWriteWordTo((u32*)SP_IMEM_START, inst.everything);
    __rmonWriteWordTo((u32*)SP_PC_REG, 0);
}
void LoadStoreVU(int opcode, int regno);

static void SetUpForRCPop(int isVector) {
    /* Save RSP data that would be overwritten when reading or writing registers */
    RCPpc = __rmonReadWordAt((u32*)SP_PC_REG);
    oldIMEMvalue = __rmonReadWordAt((u32*)SP_IMEM_START);
    DMEMbuffer[0] = __rmonReadWordAt((u32*)SP_DMEM_START);
    if (isVector) {
        DMEMbuffer[1] = __rmonReadWordAt((u32*)(SP_DMEM_START + 0x4));
        DMEMbuffer[2] = __rmonReadWordAt((u32*)(SP_DMEM_START + 0x8));
        DMEMbuffer[3] = __rmonReadWordAt((u32*)(SP_DMEM_START + 0xC));
    }
}

static void CleanupFromRCPop(int isVector) {
    /* Restore RSP data that was saved to read or write registers */
    __rmonWriteWordTo((u32*)SP_DMEM_START, DMEMbuffer[0]);
    if (isVector) {
        __rmonWriteWordTo((u32*)(SP_DMEM_START + 0x4), DMEMbuffer[1]);
        __rmonWriteWordTo((u32*)(SP_DMEM_START + 0x8), DMEMbuffer[2]);
        /* BUG: the last word is not restored properly */
        __rmonWriteWordTo((u32*)(SP_DMEM_START + 0xC), DMEMbuffer[2]);
    }
    __rmonWriteWordTo((u32*)SP_IMEM_START, oldIMEMvalue);
    __rmonWriteWordTo((u32*)SP_PC_REG, RCPpc);
}
int __rmonGetGRegisters(KKHeader* req);
int __rmonSetGRegisters(KKHeader* req);
int __rmonGetFRegisters(KKHeader* req);
int __rmonSetFRegisters(KKHeader* req);

u32 rmonGetRcpRegister(int regNumber) {
    u32 contents;

    if (__rmonRCPrunning()) {
        return 0;
    }

    SetUpForRCPop(FALSE);
    LoadStoreSU(MIPS_SW_OPCODE, regNumber);
    __rmonStepRCP();
    contents = __rmonReadWordAt((u32*)SP_DMEM_START);
    CleanupFromRCPop(FALSE);

    return contents;
}
int __rmonGetSRegs(KKHeader* req);
int __rmonSetSRegs(KKHeader* req);
int __rmonGetVRegs(KKHeader* req);
int __rmonSetVRegs(KKHeader* req);
u32 __rmonGetRegisterContents(int method, int threadNumber, int regNumber);

