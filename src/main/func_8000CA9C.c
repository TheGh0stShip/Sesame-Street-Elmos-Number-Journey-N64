struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_20_f64 { u8 pad[0x20]; f64 v; };
extern f64 D_80079FB8;
extern f64 D_80079FC0;

s32 f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f64 arg4) {
    if ((arg0 == 0) || (arg4 <= 0.0)) {
        return 0;
    }
    (*(f32 *)(arg0 + 4)) = *(f32 *)arg2;
    ((struct gd_4_f32 *)(arg0 + 4))->v = ((struct gd_4_f32 *)arg2)->v;
    ((struct gd_8_f32 *)(arg0 + 4))->v = ((struct gd_8_f32 *)arg2)->v;
    (*(f32 *)(arg0 + 16)) = *(f32 *)arg3;
    ((struct gd_4_f32 *)(arg0 + 16))->v = ((struct gd_4_f32 *)arg3)->v;
    ((struct gd_8_f32 *)(arg0 + 16))->v = ((struct gd_8_f32 *)arg3)->v;
    ((struct gd_20_f64 *)arg0)->v = D_80079FC0 / (arg4 * D_80079FB8);
    (*(s32 *)arg0) = arg1;
    return 1;
}
