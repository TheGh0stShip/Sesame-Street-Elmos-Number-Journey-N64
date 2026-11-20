#include "common.h"


s32 func_8004CBF4(s32 arg0) {
    register s32 reg_s0;

    reg_s0 = func_80042080(28);
    func_8004DE00(reg_s0, 28);
    (*(s32 *)reg_s0) = arg0;
    return reg_s0;
}
