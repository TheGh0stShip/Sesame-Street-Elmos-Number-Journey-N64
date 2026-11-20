/* __rmonGetBranchTarget: decode a branch/jump instruction and return its target, or -1 */
#define RMON_RSP 1
u32 func_800659E0(u32 *addr);                         /* __rmonReadWordAt */
u32 func_80066CA4(int method, int thread, int reg);   /* __rmonGetRegisterContents */

u32 func_8006520C(int method, int thread, char *addr) {
    int inst;

    if (method == RMON_RSP) {
        inst = func_800659E0((u32 *)addr);
    } else {
        inst = *(u32 *)addr;
    }

    switch ((inst >> 26) & 0x3F) {
        case 0:
            if (((inst >> 5) & 0x7FFF) == 0 && (inst & 0x3F) == 8) {
                return func_80066CA4(method, thread, (inst >> 21) & 0x1F);
            }
            if (((inst >> 16) & 0x1F) == 0 && (inst & 0x7FF) == 9) {
                return func_80066CA4(method, thread, (inst >> 21) & 0x1F);
            }
            break;
        case 1:
            switch ((inst >> 16) & 0x1F) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 16:
                case 17:
                case 18:
                case 19:
                    return (((inst << 0x10) >> 0xE) + addr + 4);
            }
            break;
        case 2:
        case 3:
            return (((u32)inst << 6) >> 4) + (((s32)((u32)addr + 4) >> 0x1C) << 0x1C);
        case 4:
        case 5:
        case 20:
        case 21:
            return (((inst << 0x10) >> 0xE) + addr + 4);
        case 6:
        case 7:
        case 22:
        case 23:
            if (((inst >> 16) & 0x1F) == 0) {
                return (((inst << 0x10) >> 0xE) + addr + 4);
            }
            break;
        case 16:
        case 17:
        case 18:
        case 19:
            if (((inst >> 21) & 0x1F) == 8) {
                switch ((inst >> 16) & 0x1F) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        return (((inst << 0x10) >> 0xE) + addr + 4);
                }
            }
            break;
    }
    return -1;
}
