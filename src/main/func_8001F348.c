struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
extern f64 D_8007A528;
extern f64 D_8007A530;
f32 func_80056690(f32);

s32 f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    f32 sp10;
    f64 sp18;

    (*(f32 *)arg3) = (*(f32 *)arg0) - (*(f32 *)arg1);
    ((struct gd_4_f32 *)arg3)->v = ((struct gd_4_f32 *)arg0)->v - ((struct gd_4_f32 *)arg1)->v;
    ((struct gd_8_f32 *)arg3)->v = ((struct gd_8_f32 *)arg0)->v - ((struct gd_8_f32 *)arg1)->v;
    sp10 = func_80056690(((((*(f32 *)arg3) * (*(f32 *)arg3)) + (((struct gd_4_f32 *)arg3)->v * ((struct gd_4_f32 *)arg3)->v)) + (((struct gd_8_f32 *)arg3)->v * ((struct gd_8_f32 *)arg3)->v)));
    if (sp10 >= 1e-07f) {
        sp18 = D_8007A528 / ((f64)sp10);
        (*(f32 *)arg3) = (f32)(((f64)(*(f32 *)arg3)) * sp18);
        ((struct gd_4_f32 *)arg3)->v = (f32)(((f64)((struct gd_4_f32 *)arg3)->v) * sp18);
        ((struct gd_8_f32 *)arg3)->v = (f32)(((f64)((struct gd_8_f32 *)arg3)->v) * sp18);
    } else {
    }
    if (sp10 < 1e-07f) {
        return 0;
    }
    (*(f32 *)arg4) = (((struct gd_4_f32 *)arg2)->v * ((struct gd_8_f32 *)arg3)->v) - (((struct gd_8_f32 *)arg2)->v * ((struct gd_4_f32 *)arg3)->v);
    ((struct gd_4_f32 *)arg4)->v = (((struct gd_8_f32 *)arg2)->v * (*(f32 *)arg3)) - ((*(f32 *)arg2) * ((struct gd_8_f32 *)arg3)->v);
    ((struct gd_8_f32 *)arg4)->v = ((*(f32 *)arg2) * ((struct gd_4_f32 *)arg3)->v) - (((struct gd_4_f32 *)arg2)->v * (*(f32 *)arg3));
    sp10 = func_80056690(((((*(f32 *)arg4) * (*(f32 *)arg4)) + (((struct gd_4_f32 *)arg4)->v * ((struct gd_4_f32 *)arg4)->v)) + (((struct gd_8_f32 *)arg4)->v * ((struct gd_8_f32 *)arg4)->v)));
    if (sp10 >= 1e-07f) {
        sp18 = D_8007A530 / ((f64)sp10);
        (*(f32 *)arg4) = (f32)(((f64)(*(f32 *)arg4)) * sp18);
        ((struct gd_4_f32 *)arg4)->v = (f32)(((f64)((struct gd_4_f32 *)arg4)->v) * sp18);
        ((struct gd_8_f32 *)arg4)->v = (f32)(((f64)((struct gd_8_f32 *)arg4)->v) * sp18);
    } else {
    }
    if (sp10 < 1e-07f) {
        return 0;
    }
    (*(f32 *)arg5) = (((struct gd_4_f32 *)arg3)->v * ((struct gd_8_f32 *)arg4)->v) - (((struct gd_8_f32 *)arg3)->v * ((struct gd_4_f32 *)arg4)->v);
    ((struct gd_4_f32 *)arg5)->v = (((struct gd_8_f32 *)arg3)->v * (*(f32 *)arg4)) - ((*(f32 *)arg3) * ((struct gd_8_f32 *)arg4)->v);
    ((struct gd_8_f32 *)arg5)->v = ((*(f32 *)arg3) * ((struct gd_4_f32 *)arg4)->v) - (((struct gd_4_f32 *)arg3)->v * (*(f32 *)arg4));
    return 1;
}
