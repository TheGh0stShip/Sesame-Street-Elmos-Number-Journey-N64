#include "common.h"


void func_8003B620(s32 arg0) {
    if (D_8011551C == 0) {
        D_8011551C = (s32)&D_80074BAC;
        osSendMesg(((s32)&D_8013EEF8), 0, 1);
    }
    func_80037450(arg0, 25);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    D_800A7F2C = 0;
}
