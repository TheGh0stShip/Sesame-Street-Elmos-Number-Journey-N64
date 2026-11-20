#include "common.h"


void func_80025700(s32 arg0) {
    s32 sp18;
    s32 pad0;
    s32 pad1[2];
    s8 pad2;

    sp18 = arg0;
    sp18 = arg0;
    if (D_800AD03A == 0) {
        if (D_80115520 < 0) {
            ((struct gd_C_s32 *)sp18)->v = ((struct gd_C_s32 *)sp18)->v | 1;
        }
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            ((struct gd_C_s32 *)sp18)->v = ((struct gd_C_s32 *)sp18)->v | 1;
        }
        return;
    }
    D_80115518 = D_80115518 + 4;
    D_8010E64C = (f32)(((f64)(*(f32 *)(((((u32)D_80115518 >> 3) & 255) * 4) + ((s32)&D_800741E0)))) * D_8007A7C8);
    ((struct gd_28_f32 *)sp18)->v = ((struct gd_28_f32 *)sp18)->v + D_8010E64C;
    ((s32 (*)())func_8001FAA8)(((s32)&((struct n64dk_mE8_40 *)sp18)->m[D_800717E0]), (sp18 + 36), ((struct gd_1A8_t *)sp18)->v);
    ((struct gd_28_f32 *)sp18)->v = ((struct gd_28_f32 *)sp18)->v - D_8010E64C;
    addActorDL(((s32)&D_800258C0), sp18, ((struct gd_1F0_s32 *)sp18)->v, 1, ((f32)func_800236B4((sp18 + 36))));
}
