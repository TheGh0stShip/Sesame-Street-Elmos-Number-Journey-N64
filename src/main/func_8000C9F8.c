struct gd_18_f64 { u8 pad[0x18]; f64 v; };
extern f64 D_80079FA8;
extern f64 D_80079FB0;

f64 f(s32 arg0, f64 arg2) {
    f64 sp0;

    if ((arg0 == 0) || (arg2 <= 0.0)) {
        return 0.0;
    }
    sp0 = ((struct gd_18_f64 *)arg0)->v;
    ((struct gd_18_f64 *)arg0)->v = D_80079FB0 / (arg2 * D_80079FA8);
    return sp0;
}
