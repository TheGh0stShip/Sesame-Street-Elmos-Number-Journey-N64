#include "common.h"

extern u16 D_80107CBA;

void func_8003A98C(s32 arg0) {
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        func_800375A8(arg0, 23);
    }
    if (D_800A9148 < 130) {
        ((struct gd_1F4_f32 *)arg0)->v = -2.0f;
    }
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
