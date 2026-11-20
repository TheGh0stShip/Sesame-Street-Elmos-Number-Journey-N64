struct gd_8_f64 { u8 pad[0x8]; f64 v; };
struct gd_10_f64 { u8 pad[0x10]; f64 v; };
struct gd_18_f64 { u8 pad[0x18]; f64 v; };
extern f64 D_80079F80;
extern f64 D_80079F88;

s32 f(s32 arg0, s32 arg1, f64 arg2, f64 arg4, f64 arg6) {
    if ((arg0 == 0) || (arg6 <= 0.0)) {
        return 0;
    }
    ((struct gd_8_f64 *)arg0)->v = arg2;
    ((struct gd_10_f64 *)arg0)->v = arg4;
    (*(s32 *)arg0) = arg1;
    ((struct gd_18_f64 *)arg0)->v = D_80079F88 / (arg6 * D_80079F80);
    return 1;
}
