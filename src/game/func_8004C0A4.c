#include "common.h"


s32 func_8004C0A4(s32 arg0) {
    register s32 reg_s0;

    reg_s0 = func_80042080(8);
    func_8004DE00(reg_s0, 8);
    (*(s32 *)reg_s0) = arg0;
    return reg_s0;
}
