#include "common.h"


void func_80037E60(s32 arg0) {
    func_800374D0(arg0);
    if ((*(u8 (*))&D_800A7A5D) == 0) {
        if (D_8011551C == 0) {
            D_8011551C = (s32)&D_80074380;
            osSendMesg(((s32)&D_8013EEF8), 0, 1);
        }
        func_80037450(arg0, 10);
        (*(u8 (*))&D_800A7A5D) = 1;
    } else {
        if (D_8011551C == 0) {
            D_8011551C = (s32)&D_80074390;
            osSendMesg(((s32)&D_8013EEF8), 0, 1);
        }
        func_80037450(arg0, 12);
    }
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v | 128;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 4;
    (*(f32 *)(arg0 + 536)) = *(f32 *)(arg0 + 36);
    ((struct gd_4_f32 *)(arg0 + 536))->v = ((struct gd_4_f32 *)(arg0 + 36))->v;
    ((struct gd_8_f32 *)(arg0 + 536))->v = ((struct gd_8_f32 *)(arg0 + 36))->v;
}
