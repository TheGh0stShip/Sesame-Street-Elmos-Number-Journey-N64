struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_6_s16 { u8 pad[0x6]; s16 v; };
struct gd_8_f64 { u8 pad[0x8]; f64 v; };

s32 f(s32 arg0, s32 arg1) {
    f64 sp0;
    f64 sp8;
    f64 sp10;
    f64 sp18;

    sp10 = (f64)((struct gd_6_s16 *)arg0)->v;
    sp0 = ((f64)((struct gd_4_s16 *)arg0)->v) - sp10;
    sp18 = ((f64)arg1) - ((f64)(*(s32 *)arg0));
    sp8 = (sp0 * sp18) * ((struct gd_8_f64 *)arg0)->v;
    sp10 = sp10 + sp8;
    ((struct gd_6_s16 *)arg0)->v = (s32)sp10;
    (*(s32 *)arg0) = arg1;
    return ((struct gd_6_s16 *)arg0)->v;
}
