#include "common.h"

void func_80052990(s8 *arg0, s8 arg1, s32 arg2) {
    s32 pad[2]; /* n64dk: frame padding, reconstruction scaffolding */
    s32 var_v0_7;
    s8 *var_a0_0;

    var_a0_0 = arg0;
    var_v0_7 = arg2 - 1;
    if (arg2 != 0) {
        do {
            *var_a0_0 = arg1;
            var_v0_7 -= 1;
            var_a0_0 += 1;
        } while (var_v0_7 != -1);
    }
}
