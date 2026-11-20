struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_20_f64 { u8 pad[0x20]; f64 v; };
extern f64 D_80079FC8;
extern f64 D_80079FD0;
extern f64 D_80079FD8;

s32 f(s32 arg0, s32 arg1, s32 arg2) {
    f32 sp0;
    f32 sp4;
    f32 sp8;
    f64 sp10;
    f64 sp18;
    f64 sp20;
    s32 sp28;

    sp28 = arg1 - (*(s32 *)arg0);
    if (sp28 > 0) {
        sp18 = (f64)sp28;
        (*(s32 *)arg0) = arg1;
        while (!(!(sp18 > 0.0))) {
            if (sp18 >= D_80079FC8) {
                sp18 = sp18 - D_80079FD0;
                sp10 = D_80079FD8;
            } else {
                sp10 = sp18;
                sp18 = 0.0;
            }
            sp20 = sp10 * ((struct gd_20_f64 *)arg0)->v;
            sp0 = *(f32 *)(arg0 + 4);
            sp4 = ((struct gd_4_f32 *)(arg0 + 4))->v;
            sp8 = ((struct gd_8_f32 *)(arg0 + 4))->v;
            sp0 = sp0 - (*(f32 *)(arg0 + 16));
            sp4 = sp4 - ((struct gd_4_f32 *)(arg0 + 16))->v;
            sp8 = sp8 - ((struct gd_8_f32 *)(arg0 + 16))->v;
            (*(f32 *)(arg0 + 16)) = (f32)(((f64)(*(f32 *)(arg0 + 16))) + (((f64)sp0) * sp20));
            ((struct gd_4_f32 *)(arg0 + 16))->v = (f32)(((f64)((struct gd_4_f32 *)(arg0 + 16))->v) + (((f64)sp4) * sp20));
            ((struct gd_8_f32 *)(arg0 + 16))->v = (f32)(((f64)((struct gd_8_f32 *)(arg0 + 16))->v) + (((f64)sp8) * sp20));
        }
    }
    if (arg2 != 0) {
        (*(f32 *)arg2) = *(f32 *)(arg0 + 16);
        ((struct gd_4_f32 *)arg2)->v = ((struct gd_4_f32 *)(arg0 + 16))->v;
        ((struct gd_8_f32 *)arg2)->v = ((struct gd_8_f32 *)(arg0 + 16))->v;
    }
    return 1;
}
