#include "common.h"

extern u16 D_80107CBA;

void func_800376B0(s32 arg0) {
    s32 pad0[2];
    s8 pad1;

    ((struct gd_1CC_s32 *)arg0)->v++;
    if (D_80109D4C == 0) {
        return;
    }
    if (((struct gd_204_s32 *)arg0)->v != 0) {
        func_800375A8(arg0, 1);
    } else {
        if (((D_80107CBA & 0xC00F) & 0xFFFF) != 0) {
            if (D_8012F154 != 11) {
                func_800375A8(arg0, 6);
            }
        }
    }
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
