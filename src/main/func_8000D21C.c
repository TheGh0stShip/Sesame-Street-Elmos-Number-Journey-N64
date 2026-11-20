struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_6_s16 { u8 pad[0x6]; s16 v; };
struct gd_8_f64 { u8 pad[0x8]; f64 v; };
extern f64 D_8007A000;
extern f64 D_8007A008;

s32 f(s32 arg0, s32 arg1, u16 arg2, u16 arg3, f64 arg4) {
    if ((arg0 == 0) || (arg4 <= 0.0)) {
        return 0;
    }
    ((struct gd_4_s16 *)arg0)->v = arg2;
    ((struct gd_6_s16 *)arg0)->v = arg3;
    (*(s32 *)arg0) = arg1;
    ((struct gd_8_f64 *)arg0)->v = D_8007A008 / (arg4 * D_8007A000);
    return 1;
}
