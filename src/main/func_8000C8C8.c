struct gd_8_f64 { u8 pad[0x8]; f64 v; };
struct gd_10_f64 { u8 pad[0x10]; f64 v; };
struct gd_18_f64 { u8 pad[0x18]; f64 v; };
extern f64 D_80079F90;
extern f64 D_80079F98;
extern f64 D_80079FA0;

f64 f(s32 arg0, s32 arg1) {
    f64 sp0;
    f64 sp8;
    f64 sp10;
    f64 sp18;
    s32 sp20;

    sp20 = arg1 - (*(s32 *)arg0);
    if (sp20 > 0) {
        sp18 = (f64)sp20;
        (*(s32 *)arg0) = arg1;
        while (!(!(sp18 > 0.0))) {
            if (sp18 >= D_80079F90) {
                sp18 = sp18 - D_80079F98;
                sp10 = D_80079FA0;
            } else {
                sp10 = sp18;
                sp18 = 0.0;
            }
            sp0 = ((struct gd_8_f64 *)arg0)->v - ((struct gd_10_f64 *)arg0)->v;
            sp8 = (sp0 * sp10) * ((struct gd_18_f64 *)arg0)->v;
            ((struct gd_10_f64 *)arg0)->v = ((struct gd_10_f64 *)arg0)->v + sp8;
        }
    }
    return ((struct gd_10_f64 *)arg0)->v;
}
