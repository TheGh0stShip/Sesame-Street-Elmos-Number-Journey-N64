/* libultra rmon __rmonSetSingleStep: plant temporary breakpoints for a single step */
typedef struct {
    u32 *breakAddress;
    u32 oldInstruction;
} BREAKINFO;

extern BREAKINFO D_800A3700; /* altBreak */
extern BREAKINFO D_800A3780; /* second temporary breakpoint */
extern u32 func_8006520C(s32 rcp, s32 thread, u32 *instptr); /* __rmonGetBranchTarget */
extern void func_80062D90(void *, s32); /* osWritebackDCache */
extern void func_80062BE0(void *, s32); /* osInvalICache */

#define BREAKINST 0x0000040D

static u32 IsJump(u32 inst) {
    switch (inst >> 26) {
        case 0:
            if (((inst >> 5) & 0x7FFF) == 0 && (inst & 0x3F) == 8) {
                return 1;
            }
            if (((inst >> 16) & 0x1F) == 0 && (inst & 0x7FF) == 9) {
                return 1;
            }
            break;
        case 2:
        case 3:
            return 1;
    }
    return 0;
}

static void SetTempBreakpoint(u32 *addr1, u32 *addr2) {
    D_800A3700.oldInstruction = *addr1;
    *addr1 = BREAKINST;
    func_80062D90(addr1, 4);
    func_80062BE0(addr1, 4);
    D_800A3700.breakAddress = addr1;
    if (addr2) {
        D_800A3780.oldInstruction = *addr2;
        *addr2 = BREAKINST;
        func_80062D90(addr2, 4);
        func_80062BE0(addr2, 4);
        D_800A3780.breakAddress = addr2;
    }
}

s32 func_8006538C(s32 thread, u32 *instptr) {
    u32 branchTarget = func_8006520C(0, thread, instptr);

    if (branchTarget & 3) {
        SetTempBreakpoint(instptr + 1, NULL);
    } else if (branchTarget == (u32)instptr) {
        return 0;
    } else if (IsJump(*instptr) || branchTarget == (u32)(instptr + 2)) {
        SetTempBreakpoint((u32 *)branchTarget, NULL);
    } else {
        SetTempBreakpoint((u32 *)branchTarget, instptr + 2);
    }
    return 1;
}
